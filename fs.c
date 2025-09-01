#include "fs.h"
#include <string.h>

static uint8_t* disk;
static struct ext2_superblock sb;
static struct ext2_group_desc gd;

void fs_init(uint8_t* disk_image){
    disk = disk_image;
    read_superblock(&sb);
    read_block(2, (uint8_t*)&gd); // first group descriptor
}

void read_block(uint32_t block_num, uint8_t* buffer){
    memcpy(buffer, disk + block_num*BLOCK_SIZE, BLOCK_SIZE);
}

void read_superblock(struct ext2_superblock* superblock){
    read_block(1, (uint8_t*)superblock);
}

void read_inode(uint32_t inode_num, struct ext2_inode* inode){
    uint32_t inodes_per_block = BLOCK_SIZE / sizeof(struct ext2_inode);
    uint32_t block = gd.bg_inode_table + (inode_num - 1) / inodes_per_block;
    uint32_t offset = (inode_num - 1) % inodes_per_block;
    struct ext2_inode tmp[BLOCK_SIZE/sizeof(struct ext2_inode)];
    read_block(block, (uint8_t*)tmp);
    *inode = tmp[offset];
}

void read_file(struct ext2_inode* inode, uint8_t* buffer, uint32_t size){
    uint32_t copied = 0;
    for(int i=0;i<12 && copied<size;i++){
        uint32_t block = inode->i_block[i];
        if(block==0) break;
        uint32_t to_copy = (size - copied > BLOCK_SIZE) ? BLOCK_SIZE : size - copied;
        read_block(block, buffer+copied);
        copied += to_copy;
    }
    // indirect blocks can be added later
}
#include "fs.h"
#include <string.h>

// directories and call stuff
void ext2_list_dir(struct ext2_inode* dir_inode, void (*callback)(const char*, uint32_t)){
    uint8_t block[BLOCK_SIZE];
    for(int i=0;i<12;i++){
        if(dir_inode->i_block[i]==0) continue;
        read_block(dir_inode->i_block[i], block);
        uint32_t offset=0;
        while(offset<dir_inode->i_size){
            struct ext2_dir_entry* entry = (struct ext2_dir_entry*)(block+offset);
            if(entry->inode==0) break;
            char name[256];
            for(int n=0;n<entry->name_len;n++) name[n] = entry->name[n];
            name[entry->name_len]=0;
            callback(name, entry->inode);
            offset += entry->rec_len;
        }
    }
}

// why.
uint32_t ext2_lookup(struct ext2_inode* dir_inode, const char* name){
    uint8_t block[BLOCK_SIZE];
    for(int i=0;i<12;i++){
        if(dir_inode->i_block[i]==0) continue;
        read_block(dir_inode->i_block[i], block);
        uint32_t offset=0;
        while(offset<dir_inode->i_size){
            struct ext2_dir_entry* entry = (struct ext2_dir_entry*)(block+offset);
            if(entry->inode==0) break;
            char entry_name[256];
            for(int n=0;n<entry->name_len;n++) entry_name[n]=entry->name[n];
            entry_name[entry->name_len]=0;
            if(strcmp(entry_name,name)==0) return entry->inode;
            offset += entry->rec_len;
        }
    }
    return 0; // not found
}
#include "fs.h"
#include <string.h>

void read_file(struct ext2_inode* inode, uint8_t* buffer, uint32_t size){
    uint32_t copied = 0;
    uint8_t block[BLOCK_SIZE];

    // THE DIRECT BLOCKS YOU IDIOT
    for(int i=0;i<12 && copied<size;i++){
        if(inode->i_block[i]==0) break;
        uint32_t to_copy = (size - copied > BLOCK_SIZE) ? BLOCK_SIZE : size - copied;
        read_block(inode->i_block[i], buffer+copied);
        copied += to_copy;
    }

    // single indirect
    if(inode->i_block[12]!=0 && copied<size){
        read_block(inode->i_block[12], block);
        uint32_t* indirect = (uint32_t*)block;
        for(int i=0;i<BLOCK_SIZE/4 && copied<size;i++){
            if(indirect[i]==0) break;
            uint32_t to_copy = (size - copied > BLOCK_SIZE) ? BLOCK_SIZE : size - copied;
            read_block(indirect[i], buffer+copied);
            copied += to_copy;
        }
    }
}

