#ifndef FS_H
#define FS_H
#include <stdint.h>

#define BLOCK_SIZE 1024

struct ext2_superblock {
    uint32_t s_inodes_count;
    uint32_t s_blocks_count;
    uint32_t s_free_blocks_count;
    uint32_t s_free_inodes_count;
    uint32_t s_first_data_block;
    uint32_t s_log_block_size;
};

struct ext2_group_desc {
    uint32_t bg_block_bitmap;
    uint32_t bg_inode_bitmap;
    uint32_t bg_inode_table;
    uint16_t bg_free_blocks_count;
    uint16_t bg_free_inodes_count;
    uint16_t bg_used_dirs_count;
    uint16_t bg_pad;
    uint32_t bg_reserved[3];
};

struct ext2_inode {
    uint16_t i_mode;
    uint16_t i_uid;
    uint32_t i_size;
    uint32_t i_atime;
    uint32_t i_ctime;
    uint32_t i_mtime;
    uint32_t i_dtime;
    uint16_t i_gid;
    uint16_t i_links_count;
    uint32_t i_blocks;
    uint32_t i_flags;
    uint32_t i_osd1;
    uint32_t i_block[15]; // 12 direct + 3 indirect
    uint32_t i_generation;
    uint32_t i_file_acl;
    uint32_t i_dir_acl;
    uint32_t i_faddr;
    uint8_t  i_osd2[12];
};

struct ext2_dir_entry {
    uint32_t inode;
    uint16_t rec_len;
    uint8_t  name_len;
    uint8_t  file_type;
    char     name[];
};

void fs_init(uint8_t* disk_image);
void read_block(uint32_t block_num, uint8_t* buffer);
void read_superblock(struct ext2_superblock* sb);
void read_inode(uint32_t inode_num, struct ext2_inode* inode);
void read_file(struct ext2_inode* inode, uint8_t* buffer, uint32_t size);

#endif

#ifndef FS_H
#define FS_H
#include <stdint.h>

// ... existing structs

uint32_t ext2_lookup(struct ext2_inode* dir_inode, const char* name);
void ext2_list_dir(struct ext2_inode* dir_inode, void (*callback)(const char*, uint32_t));
