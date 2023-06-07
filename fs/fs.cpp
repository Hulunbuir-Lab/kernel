#include <fat32.h>

struct file fileTable[OPENFILENUM];
unsigned int rootCluster;
struct superBlock SUPERBLOCK;
unsigned int *fatTable;
#define printd(str0) uPut<<str0<<"\n"
static xfat_err_t to_sfn(char* dest_name, const char* my_name) {
    int i, name_len;
    char * dest = dest_name;
    const char * ext_dot;
    const char * p;
    int ext_existed;

    memset(dest, ' ', SFN_LEN);

    // 跳过开头的分隔符
    while (is_path_sep(*my_name)) {
        my_name++;
    }

    // 找到第一个斜杠之前的字符串，将ext_dot定位到那里，且记录有效长度
    ext_dot = my_name;
    p = my_name;
    name_len = 0;
    while ((*p != '\0') && !is_path_sep(*p)) {
        if (*p == '.') {
            ext_dot = p;
        }
        p++;
        name_len++;
    }

    // 如果文件名以.结尾，意思就是没有扩展名？
    // todo: 长文件名处理?
    ext_existed = (ext_dot > my_name) && (ext_dot < (my_name + name_len - 1));

    // 遍历名称，逐个复制字符, 算上.分隔符，最长12字节，如果分离符，则只应有
    p = my_name;
    for (i = 0; (i < SFN_LEN) && (*p != '\0') && !is_path_sep(*p); i++) {
        if (ext_existed) {
            if (p == ext_dot) {
                dest = dest_name + 8;
                p++;
                i--;
                continue;
            }
            else if (p < ext_dot) {
                *dest++ = toupper(*p++);
            }
            else {
                *dest++ = toupper(*p++);
            }
        }
        else {
            *dest++ = toupper(*p++);
        }
    }
    return FS_ERR_OK;
}


/**
 * 检查sfn字符串中是否是大写。如果中间有任意小写，都认为是小写
 * @param name
 * @return
 */
static u8 get_sfn_case_cfg(const char * sfn_name) {
    u8 case_cfg = 0;

    int name_len;
    const char * src_name = sfn_name;
    const char * ext_dot;
    const char * p;
    int ext_existed;

    // 跳过开头的分隔符
    while (is_path_sep(*src_name)) {
        src_name++;
    }

    // 找到第一个斜杠之前的字符串，将ext_dot定位到那里，且记录有效长度
    ext_dot = src_name;
    p = src_name;
    name_len = 0;
    while ((*p != '\0') && !is_path_sep(*p)) {
        if (*p == '.') {
            ext_dot = p;
        }
        p++;
        name_len++;
    }

    // 如果文件名以.结尾，意思就是没有扩展名？
    // todo: 长文件名处理?
    ext_existed = (ext_dot > src_name) && (ext_dot < (src_name + name_len - 1));
    for (p = src_name; p < src_name + name_len; p++) {
        if (ext_existed) {
            if (p < ext_dot) { // 文件名主体部分大小写判断
                case_cfg |= islower(*p) ? DIRITEM_NTRES_BODY_LOWER : 0;
            } else if (p > ext_dot) {
                case_cfg |= islower(*p) ? DIRITEM_NTRES_EXT_LOWER : 0;
            }
        } else {
            case_cfg |= islower(*p) ? DIRITEM_NTRES_BODY_LOWER : 0;
        }
    }

    return case_cfg;
}

/**
 * 判断两个文件名是否匹配
 * @param name_in_item fatdir中的文件名格式
 * @param my_name 应用可读的文件名格式
 * @return
 */
static u8 is_filename_match(const char *name_in_dir, const char *to_find_name) {
    char temp_name[SFN_LEN];

    // FAT文件名的比较检测等，全部转换成大写比较
    // 根据目录的大小写配置，将其转换成8+3名称，再进行逐字节比较
    // 但实际显示时，会根据diritem->NTRes进行大小写转换
    to_sfn(temp_name, to_find_name);
    return memcmp(temp_name, name_in_dir, SFN_LEN) == 0;
}
static const char * skip_first_path_sep (const char * path) {
    const char * c = path;

    if (c == (const char *)0) {
        return (const char *)0;
    }

    // 跳过开头的分隔符
    while (is_path_sep(*c)) {
        c++;
    }
    return c;
}

/**
 * 获取子路径
 * @param dir_path 上一级路径
 * @return
 */
const char * get_child_path(const char *dir_path) {
    const char * c = skip_first_path_sep(dir_path);

    // 跳过父目录
    while ((*c != '\0') && !is_path_sep(*c)) {
        c++;
    }

    return (*c == '\0') ? (const char *)0 : c + 1;
}

static xfile_type_t get_file_type(const dirEntry *diritem) {
    xfile_type_t type;

    if (diritem->DIR_Attr & DIRITEM_ATTR_VOLUME_ID) {
        type = FAT_VOL;
    } else if (diritem->DIR_Attr & DIRITEM_ATTR_DIRECTORY) {
        type = FAT_DIR;
    } else {
        type = FAT_FILE;
    }

    return type;
}
int clusterLevel = 0;
void fat32_mount(){
    char buf[512];
    memset(buf,0,512);
    sdcard.ReadBlock(0,buf);
    SUPERBLOCK.bytesPerSector = *(unsigned short *)(buf + bytesPerSectorInex);
    SUPERBLOCK.sectorPerCluster = *(unsigned char *)(buf + sectorPerClusterIndex);
    SUPERBLOCK.reserverSector = *(unsigned short *)(buf + reserverSectorIndex);
    SUPERBLOCK.fatNum = *(unsigned char *)(buf+fatNumIndex);
    SUPERBLOCK.sectorPerFat = *(unsigned int *)(buf + sectorPerFatIndex);
    rootCluster = *(unsigned int *)(buf + rootClusterIndex);
    int level =  0;
    // cal fat size
    int sectors = 0x1000 / 0x200;  //8
    for(;sectors<=SUPERBLOCK.sectorPerFat; level++,sectors*=2)
        ;
    int pageSize = 0x1000;
    fatTable = (unsigned int *)pageAllocator.AllocPageMem(level);
    for(int i=0; i<SUPERBLOCK.sectorPerFat; i++)
        sdcard.ReadBlock((SUPERBLOCK.reserverSector+i)*512,(void *)(fatTable+512/sizeof(u32)*i));
    for(;pageSize >= SUPERBLOCK.sectorPerCluster*0x200; pageSize*=2)
        ;
    for(int i=0; i<OPENFILENUM; i++){
        fileTable[i].count = 0;
    }
    uPut<<"secClus="<<SUPERBLOCK.sectorPerCluster*512<<"\n";
}


u32 getDirEntryCluster(struct dirEntry *dirEntry0){
    return (dirEntry0->DIR_FstClusHI << 16) + dirEntry0->DIR_FstClusL0;
}
int is_cluster_valid(u32 cluster) {
    cluster &= 0x0FFFFFFF;
    return (cluster < 0x0FFFFFF0) && (cluster >= 0x2);     // 值是否正确
}
u32 clusterFirstSector(u32 clusterNum){
    u32 dataStartSector = SUPERBLOCK.reserverSector + SUPERBLOCK.fatNum * SUPERBLOCK.sectorPerFat;
    u32 sectorNum = dataStartSector + (clusterNum-2) *SUPERBLOCK.sectorPerCluster;
    return sectorNum;
}
u32 getNextCluster(u32 curClusrerNum){
    return fatTable[curClusrerNum];
}
char bufPublic[512];
int findEntry(u32 *parentCluster, u32 *parentClusterOffet, const char *curPath, struct dirEntry *dirEntry0){
    u32 currCluster = *parentCluster;
    u32 initialSector = 0;
    do{
        u32 startSector = clusterFirstSector(currCluster);
        u32 i;
        for(i=0; i<SUPERBLOCK.sectorPerCluster; i++){
            u32 j;
            memset(bufPublic,0,512);
            sdcard.ReadBlock((startSector+i)*512,bufPublic);
            for(j=0; j<512/sizeof(struct dirEntry); j++){
                struct dirEntry *walkEntry = (struct dirEntry *)bufPublic + j;
                if(walkEntry->DIR_Name[0] == DIRITEM_NAME_END){
                    //uPut<<"a i="<<i<<"    "<<"j="<<j<<"\n";
                    return FS_ERR_EOF;
                }
                if(walkEntry->DIR_Name[0] == DIRITEM_NAME_FREE){
                    //uPut<<"b i="<<i<<"    "<<"j="<<j<<"\n";
                    continue;
                }
                if(curPath == NULL || *curPath == 0 || is_filename_match((const char *)walkEntry->DIR_Name,curPath)){
                    //uPut<<"c i="<<i<<"    "<<"j="<<j<<"\n";
                    *parentCluster = currCluster;
                    *parentClusterOffet = i*512+j*sizeof(struct dirEntry);
                    memcpy(dirEntry0,walkEntry,sizeof(struct dirEntry));
                    return FS_ERR_OK;
                }
                uPut<<"d i="<<i<<"    "<<"j="<<j<<"\n";
            }
        }
        currCluster = getNextCluster(currCluster);

    }while(is_cluster_valid(currCluster));
    return FS_ERR_EOF;
}
void readCluster(u8 *bufClusrer,u32 clusterNum){
    int startSector = clusterFirstSector(clusterNum);
    uPut<<"startsector="<<startSector<<"\n";
    for(int i=0; i<SUPERBLOCK.sectorPerCluster; i++){
        uPut<<"i="<<i<<"\n";
        uPut<<"(startSector+i)*512="<<(startSector+i)*512<<"\n";
        sdcard.ReadBlock((startSector+i)*512,(u8 *)bufClusrer+512*i);
        uPut<<"ppppppppppppp"<<"\n";
        uPut<<"qqqqqqqqqqqq"<<"\n";
        //uPut<<bufClusrer[0]<<"\n";
        uPut<<"qqqqqqqqqqqq"<<"\n";
    }
}
int openSubFile(u32 dirCluster, struct file *file0,const char *path){
    u32 parentCluster = dirCluster;
    u32 parentClusterOffset = 0;
    path = skip_first_path_sep(path);
    if((path != 0) && (*path != '\0')){
        struct dirEntry dirEntry0;
        u32 fileStartCluster = 0;
        const char *curPath = path;
        while(curPath != (const char *)0){
            int res = -1;
            res = findEntry(&parentCluster,&parentClusterOffset,curPath,&dirEntry0);
            if(res == -1)
                return -1;
            curPath = get_child_path(curPath);
            if(curPath != NULL){
                parentCluster = getDirEntryCluster(&dirEntry0);
                parentClusterOffset = 0;
            }else{
                fileStartCluster = getDirEntryCluster(&dirEntry0);
            }
        }
        file0->size = dirEntry0.DIR_FileSize;
        file0->type = get_file_type(&dirEntry0);
        file0->attr = dirEntry0.DIR_Attr;
        file0->start_cluster = getDirEntryCluster(&dirEntry0);
        file0->curr_cluster = fileStartCluster;
        file0->dir_cluster = parentCluster;
        file0->dir_cluster_offset = parentClusterOffset;
    }else{
        file0->size = 0;
        file0->type = FAT_DIR;
        file0->attr = 0;
        file0->start_cluster = parentCluster;
        file0->curr_cluster = parentCluster;
        file0->dir_cluster = CLUSTER_INVALID;
        file0->dir_cluster_offset = 0;
    }
    file0->pos = 0;
    return 0;
}
int open(const char *path,struct file *file0){
    // int fd = -1;
    // for(int i=0; i<OPENFILENUM; i++){
    //     if(fileTable[i].count == 0){
    //         fd = i;
    //         break;
    //     }
    // }
    // if(fd == -1)
    //     return -1;
    // path = get_child_path(path);

    if (!is_path_end(path)) {
        path = skip_first_path_sep(path);

        // 根目录不存在上级目录
        // 若含有.，直接过滤掉路径
        if (memcmp(path, "..", 2) == 0) {
            return -1;
        } else if (memcmp(path, ".", 1) == 0) {
            path++;
        }
    }
    openSubFile(rootCluster,file0,path);
    return 0;
}
#define MAXFILESIZE 1024*1024
u8 bufTemp[MAXFILESIZE];
int read(struct file *file0, unsigned char *bufDst, int count){
    u32 alreadyReaded = 0;
    if(file0->pos > file0->size){
        return 0;
    }
    if(file0->pos + count > file0->size){
        count = file0->size - file0->pos; 
    }
    int filesize = file0->size,pageSize = 0x1000;
    int level = 0;
    for(;pageSize>=filesize; level++)
        pageSize*=2;
    //bufTemp = (u8 *)pageAllocator.AllocPageMem(level);
    memset(bufTemp,0,MAXFILESIZE);
    uPut<<"filesize="<<filesize<<"\n";
    uPut<<"SUPERBLOCK.sectorPerCluster*512="<<SUPERBLOCK.sectorPerCluster*512<<"\n";
    int clusterCount = filesize / (SUPERBLOCK.sectorPerCluster*512);
    uPut<<"clustercount="<<clusterCount<<"\n";
    clusterCount = (filesize % (SUPERBLOCK.sectorPerCluster*512) == 0)? (clusterCount):(clusterCount+1);
    int cluterNum = file0->start_cluster;
    uPut<<"clusterNum="<<cluterNum<<"\n";
    uPut<<"clustercount="<<clusterCount<<"\n";
    for(int i=0; i<clusterCount && is_cluster_valid(cluterNum); i++){
        readCluster(bufTemp+i*SUPERBLOCK.sectorPerCluster*512,cluterNum);
        cluterNum = fatTable[cluterNum];
    }
    memcpy(bufDst,bufTemp+file0->pos,count);
    return count;
}
void printStr(u8 *str0){
    int i=0;
    while(str0[i]){
        uPut<<(char )str0[i];
        i++;
    }
    uPut<<"\n";
}
u8 buf1[4096*4];
void fstest0(){
    struct file file0;
    memset(&file0,0,sizeof(struct file));
    uPut<<file0.size<<"\n";
    uPut<<"start0"<<"\n";
    open("/qhl",&file0);
    uPut<<"finish0"<<"\n";
    uPut<<file0.size<<"\n";
    memset(buf1,0,4096*4);
    read(&file0,buf1,15000);
    uPut<<buf1<<"\n";
    printStr(buf1);
}
