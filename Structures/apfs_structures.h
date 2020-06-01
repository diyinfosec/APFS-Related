struct prange {
  paddr_t pr_start_paddr;
  uint64_t pr_block_count;
};

struct obj_phys {
  uint8_t o_cksum[MAX_CKSUM_SIZE];
  oid_t o_oid;
  xid_t o_xid;
  uint32_t o_type;
  uint32_t o_subtype;
};

struct checkpoint_map_phys {
  obj_phys_t cpm_o;
  uint32_t cpm_flags;
  uint32_t cpm_count;
  checkpoint_mapping_t cpm_map[];
};

struct evict_mapping_val {
  paddr_t dst_paddr;
  uint64_t len;
}
__attribute__((packed));

struct omap_phys {
  obj_phys_t om_o;
  uint32_t om_flags;
  uint32_t om_snap_count;
  uint32_t om_tree_type;
  uint32_t om_snapshot_tree_type;
  oid_t om_tree_oid;
  oid_t om_snapshot_tree_oid;
  xid_t om_most_recent_snap;
  xid_t om_pending_revert_min;
  xid_t om_pending_revert_max;
};

struct omap_key {
  oid_t ok_oid;
  xid_t ok_xid;
};

struct omap_val {
  uint32_t ov_flags;
  uint32_t ov_size;
  paddr_t ov_paddr;
};

struct omap_snapshot {
  uint32_t oms_flags;
  uint32_t oms_pad;
  oid_t oms_oid;
};

struct apfs_superblock {
  obj_phys_t apfs_o;
  uint32_t apfs_magic;
  uint32_t apfs_fs_index;
  uint64_t apfs_features;
  uint64_t apfs_readonly_compatible_features;
  uint64_t apfs_incompatible_features;
  uint64_t apfs_unmount_time;
  uint64_t apfs_fs_reserve_block_count;
  uint64_t apfs_fs_quota_block_count;
  uint64_t apfs_fs_alloc_count;
  wrapped_meta_crypto_state_t apfs_meta_crypto;
  uint32_t apfs_root_tree_type;
  uint32_t apfs_extentref_tree_type;
  uint32_t apfs_snap_meta_tree_type;
  oid_t apfs_omap_oid;
  oid_t apfs_root_tree_oid;
  oid_t apfs_extentref_tree_oid;
  oid_t apfs_snap_meta_tree_oid;
  xid_t apfs_revert_to_xid;
  oid_t apfs_revert_to_sblock_oid;
  uint64_t apfs_next_obj_id;
  uint64_t apfs_num_files;
  uint64_t apfs_num_directories;
  uint64_t apfs_num_symlinks;
  uint64_t apfs_num_other_fsobjects;
  uint64_t apfs_num_snapshots;
  uint64_t apfs_total_blocks_alloced;
  uint64_t apfs_total_blocks_freed;
  uuid_t apfs_vol_uuid;
  uint64_t apfs_last_mod_time;
  uint64_t apfs_fs_flags;
  apfs_modified_by_t apfs_formatted_by;
  apfs_modified_by_t apfs_modified_by[APFS_MAX_HIST];
  uint8_t apfs_volname[APFS_VOLNAME_LEN];
  uint32_t apfs_next_doc_id;
  uint16_t apfs_role;
  uint16_t reserved;
  xid_t apfs_root_to_xid;
  oid_t apfs_er_state_oid;
  uint64_t apfs_cloneinfo_id_epoch;
  uint64_t apfs_cloneinfo_xid;
  oid_t apfs_snap_meta_ext_oid;
  uuid_t apfs_volume_group_id;
};

struct apfs_modified_by {
  uint8_t id[APFS_MODIFIED_NAMELEN];
  uint64_t timestamp;
  xid_t last_xid;
};

struct j_key {
  uint64_t obj_id_and_type;
}
__attribute__((packed));

struct j_inode_key {
  j_key_t hdr;
}
__attribute__((packed));

struct j_inode_val {
  uint64_t parent_id;
  uint64_t private_id;
  uint64_t create_time;
  uint64_t mod_time;
  uint64_t change_time;
  uint64_t access_time;
  uint64_t internal_flags;
  union {
    int32_t nchildren;
    int32_t nlink;
  };
  cp_key_class_t default_protection_class;
  uint32_t write_generation_counter;
  uint32_t bsd_flags;
  uid_t owner;
  gid_t group;
  mode_t mode;
  uint16_t pad1;
  uint64_t uncompressed_size;
  uint8_t xfields[];
}
__attribute__((packed));

struct j_drec_key {
  j_key_t hdr;
  uint16_t name_len;
  uint8_t name[0];
}
__attribute__((packed));

struct j_drec_hashed_key {
  j_key_t hdr;
  uint32_t name_len_and_hash;
  uint8_t name[0];
}
__attribute__((packed));

struct j_drec_val {
  uint64_t file_id;
  uint64_t date_added;
  uint16_t flags;
  uint8_t xfields[];
}
__attribute__((packed));

struct j_dir_stats_key {
  j_key_t hdr;
}
__attribute__((packed));

struct j_dir_stats_val {
  uint64_t num_children;
  uint64_t total_size;
  uint64_t chained_key;
  uint64_t gen_count;
}
__attribute__((packed));

struct j_xattr_key {
  j_key_t hdr;
  uint16_t name_len;
  uint8_t name[0];
}
__attribute__((packed));

struct j_xattr_val {
  uint16_t flags;
  uint16_t xdata_len;
  uint8_t xdata[0];
}
__attribute__((packed));

typedef enum {
  APFS_TYPE_ANY = 0, APFS_TYPE_SNAP_METADATA = 1, APFS_TYPE_EXTENT = 2, APFS_TYPE_INODE = 3, APFS_TYPE_XATTR = 4, APFS_TYPE_SIBLING_LINK = 5, APFS_TYPE_DSTREAM_ID = 6, APFS_TYPE_CRYPTO_STATE = 7, APFS_TYPE_FILE_EXTENT = 8, APFS_TYPE_DIR_REC = 9, APFS_TYPE_DIR_STATS = 10, APFS_TYPE_SNAP_NAME = 11, APFS_TYPE_SIBLING_MAP = 12, APFS_TYPE_MAX_VALID = 12, APFS_TYPE_MAX = 15, APFS_TYPE_INVALID = 15,
}
j_obj_types;

typedef enum {
  APFS_KIND_ANY = 0, APFS_KIND_NEW = 1, APFS_KIND_UPDATE = 2, APFS_KIND_DEAD = 3, APFS_KIND_UPDATE_REFCNT = 4, APFS_KIND_INVALID = 255
}
j_obj_kinds;

typedef enum {
  INODE_IS_APFS_PRIVATE = 0x00000001, INODE_MAINTAIN_DIR_STATS = 0x00000002, INODE_DIR_STATS_ORIGIN = 0x00000004, INODE_PROT_CLASS_EXPLICIT = 0x00000008, INODE_WAS_CLONED = 0x00000010, INODE_FLAG_UNUSED = 0x00000020, INODE_HAS_SECURITY_EA = 0x00000040, INODE_BEING_TRUNCATED = 0x00000080, INODE_HAS_FINDER_INFO = 0x00000100, INODE_IS_SPARSE = 0x00000200, INODE_WAS_EVER_CLONED = 0x00000400, INODE_ACTIVE_FILE_TRIMMED = 0x00000800, INODE_PINNED_TO_MAIN = 0x00001000, INODE_PINNED_TO_TIER2 = 0x00002000, INODE_HAS_RSRC_FORK = 0x00004000, INODE_NO_RSRC_FORK = 0x00008000, INODE_ALLOCATION_SPILLEDOVER = 0x00010000, INODE_FAST_PROMOTE = 0x00020000, INODE_HAS_UNCOMPRESSED_SIZE = 0x00040000, INODE_IS_PURGEABLE = 0x00080000, INODE_WANTS_TO_BE_PURGEABLE = 0x00100000, INODE_IS_SYNC_ROOT = 0x00200000, INODE_INHERITED_INTERNAL_FLAGS = (INODE_MAINTAIN_DIR_STATS), INODE_CLONED_INTERNAL_FLAGS = (INODE_HAS_RSRC_FORK\ | INODE_NO_RSRC_FORK\ | INODE_HAS_FINDER_INFO),
}
j_inode_flags;

typedef enum {
  XATTR_DATA_STREAM = 0x00000001, XATTR_DATA_EMBEDDED = 0x00000002, XATTR_FILE_SYSTEM_OWNED = 0x00000004, XATTR_RESERVED_8 = 0x00000008,
}
j_xattr_flags;

typedef enum {
  DREC_TYPE_MASK = 0x000f, RESERVED_10 = 0x0010
}
dir_rec_flags;

struct j_phys_ext_key {
  j_key_t hdr;
}
__attribute__((packed));

struct j_phys_ext_val {
  uint64_t len_and_kind;
  uint64_t owning_obj_id;
  int32_t refcnt;
}
__attribute__((packed));

struct j_file_extent_key {
  j_key_t hdr;
  uint64_t logical_addr;
}
__attribute__((packed));

struct j_file_extent_val {
  uint64_t len_and_flags;
  uint64_t phys_block_num;
  uint64_t crypto_id;
}
__attribute__((packed));

struct j_dstream_id_key {
  j_key_t hdr;
}
__attribute__((packed));

struct j_dstream_id_val {
  uint32_t refcnt;
}
__attribute__((packed));

struct j_xattr_dstream {
  uint64_t xattr_obj_id;
  j_dstream_t dstream;
};

struct j_dstream {
  uint64_t size;
  uint64_t alloced_size;
  uint64_t default_crypto_id;
  uint64_t total_bytes_written;
  uint64_t total_bytes_read;
}
__attribute__((aligned(8), packed));

struct x_field {
  uint8_t x_type;
  uint8_t x_flags;
  uint16_t x_size;
};

struct j_sibling_key {
  j_key_t hdr;
  uint64_t sibling_id;
}
__attribute__((packed));

struct j_sibling_val {
  uint64_t parent_id;
  uint16_t name_len;
  uint8_t name[0];
}
__attribute__((packed));

struct j_sibling_map_key {
  j_key_t hdr;
}
__attribute__((packed));

struct j_sibling_map_val {
  uint64_t file_id;
}
__attribute__((packed));

struct j_snap_metadata_key {
  j_key_t hdr;
}
__attribute__((packed));

struct j_snap_metadata_val {
  oid_t extentref_tree_oid;
  oid_t sblock_oid;
  uint64_t create_time;
  uint64_t change_time;
  uint64_t inum;
  uint32_t extentref_tree_type;
  uint32_t flags;
  uint16_t name_len;
  uint8_t name[0];
}
__attribute__((packed));

struct j_snap_name_key {
  j_key_t hdr;
  uint16_t name_len;
  uint8_t name[0];
}
__attribute__((packed));

struct j_snap_name_val {
  xid_t snap_xid;
}
__attribute__((packed));

typedef enum {
  SNAP_META_PENDING_DATALESS = 0x00000001, SNAP_META_MERGE_IN_PROGRESS = 0x00000002,
}
snap_meta_flags;

struct snap_meta_ext_obj_phys {
  obj_phys_t smeop_o;
  snap_meta_ext_t smeop_sme;
}

typedef struct snap_meta_ext {
  uint32_t sme_version;
  uint32_t sme_flags;
  xid_t sme_snap_xid;
  uuid_t sme_uuid;
  uint64_t sme_token;
}
__attribute__((packed))

struct btree_node_phys {
  obj_phys_t btn_o;
  uint16_t btn_flags;
  uint16_t btn_level;
  uint32_t btn_nkeys;
  nloc_t btn_table_space;
  nloc_t btn_free_space;
  nloc_t btn_key_free_list;
  nloc_t btn_val_free_list;
  uint64_t btn_data[];
};

struct btree_info_fixed {
  uint32_t bt_flags;
  uint32_t bt_node_size;
  uint32_t bt_key_size;
  uint32_t bt_val_size;
};

struct btree_info {
  btree_info_fixed_t bt_fixed;
  uint32_t bt_longest_key;
  uint32_t bt_longest_val;
  uint64_t bt_key_count;
  uint64_t bt_node_count;
};

struct nloc {
  uint16_t off;
  uint16_t len;
};

struct kvloc {
  nloc_t k;
  nloc_t v;
};

struct kvoff {
  uint16_t k;
  uint16_t v;
};

struct j_crypto_key {
  j_key_t hdr;
}
__attribute__((packed));

struct j_crypto_val {
  uint32_t refcnt;
  wrapped_crypto_state_t state;
}
__attribute__((aligned(4), packed));

struct wrapped_crypto_state {
  uint16_t major_version;
  uint16_t minor_version;
  crypto_flags_t cpflags;
  cp_key_class_t persistent_class;
  cp_key_os_version_t key_os_version;
  cp_key_revision_t key_revision;
  uint16_t key_len;
  uint8_t persistent_key[0];
}
__attribute__((aligned(2), packed));

struct wrapped_meta_crypto_state {
  uint16_t major_version;
  uint16_t minor_version;
  crypto_flags_t cpflags;
  cp_key_class_t persistent_class;
  cp_key_os_version_t key_os_version;
  cp_key_revision_t key_revision;
  uint16_t unused;
}
__attribute__((aligned(2), packed));

struct kb_locker {
  uint16_t kl_version;
  uint16_t kl_nkeys;
  uint32_t kl_nbytes;
  uint8_t padding[8];
  keybag_entry_t kl_entries[];
};

struct keybag_entry {
  uuid_t ke_uuid;
  uint16_t ke_tag;
  uint16_t ke_keylen;
  uint8_t padding[4];
  uint8_t ke_keydata[];
};

struct media_keybag {
  obj_phys_t mk_obj;
  kb_locker_t mk_locker;
};

struct chunk_info {
  uint64_t ci_xid;
  uint64_t ci_addr;
  uint32_t ci_block_count;
  uint32_t ci_free_count;
  paddr_t ci_bitmap_addr;
};

struct chunk_info_block {
  obj_phys_t cib_o;
  uint32_t cib_index;
  uint32_t cib_chunk_info_count;
  chunk_info_t cib_chunk_info[];
};

struct cib_addr_block {
  obj_phys_t cab_o;
  uint32_t cab_index;
  uint32_t cab_cib_count;
  paddr_t cab_cib_addr[];
};

struct spaceman_free_queue_key {
  xid_t sfqk_xid;
  paddr_t sfqk_paddr;
};

struct spaceman_free_queue {
  uint64_t sfq_count;
  oid_t sfq_tree_oid;
  xid_t sfq_oldest_xid;
  uint16_t sfq_tree_node_limit;
  uint16_t sfq_pad16;
  uint32_t sfq_pad32;
  uint64_t sfq_reserved;
};

struct spaceman_device {
  uint64_t sm_block_count;
  uint64_t sm_chunk_count;
  uint32_t sm_cib_count;
  uint32_t sm_cab_count;
  uint64_t sm_free_count;
  uint32_t sm_addr_offset;
  uint32_t sm_reserved;
  uint64_t sm_reserved2;
};

struct spaceman_allocation_zone_boundaries {
  uint64_t saz_zone_start;
  uint64_t saz_zone_end;
};

struct spaceman_allocation_zone_info_phys {
  spaceman_allocation_zone_boundaries_t saz_current_boundaries;
  spaceman_allocation_zone_boundaries_t saz_previous_boundaries[SM_ALLOCZONE_NUM_PREVIOUS_BOUNDARIES];
  uint16_t saz_zone_id;
  uint16_t saz_previous_boundary_index;
  uint32_t saz_reserved;
};

struct spaceman_datazone_info_phys {
  spaceman_allocation_zone_info_phys_t sdz_allocation_zones[SD_COUNT][SM_DATAZONE_ALLOCZONE_COUNT];
};

struct spaceman_phys {
  obj_phys_t sm_o;
  uint32_t sm_block_size;
  uint32_t sm_blocks_per_chunk;
  uint32_t sm_chunks_per_cib;
  uint32_t sm_cibs_per_cab;
  spaceman_device_t sm_dev[SD_COUNT];
  uint32_t sm_flags;
  uint32_t sm_ip_bm_tx_multiplier;
  uint64_t sm_ip_block_count;
  uint32_t sm_ip_bm_size_in_blocks;
  uint32_t sm_ip_bm_block_count;
  paddr_t sm_ip_bm_base;
  paddr_t sm_ip_base;
  uint64_t sm_fs_reserve_block_count;
  uint64_t sm_fs_reserve_alloc_count;
  spaceman_free_queue_t sm_fq[SFQ_COUNT];
  uint16_t sm_ip_bm_free_head;
  uint16_t sm_ip_bm_free_tail;
  uint32_t sm_ip_bm_xid_offset;
  uint32_t sm_ip_bitmap_offset;
  uint32_t sm_ip_bm_free_next_offset;
  uint32_t sm_version;
  uint32_t sm_struct_size;
  spaceman_datazone_info_phys_t sm_datazone;
};

struct nx_reaper_phys {
  obj_phys_t nr_o;
  uint64_t nr_next_reap_id;
  uint64_t nr_completed_id;
  oid_t nr_head;
  oid_t nr_tail;
  uint32_t nr_flags;
  uint32_t nr_rlcount;
  uint32_t nr_type;
  uint32_t nr_size;
  oid_t nr_fs_oid;
  oid_t nr_oid;
  xid_t nr_xid;
  uint32_t nr_nrle_flags;
  uint32_t uint8_t nr_state_buffer_size;
  nr_state_buffer[];
};

struct nx_reap_list_phys {
  obj_phys_t nrl_o;
  oid_t nrl_next;
  uint32_t nrl_flags;
  uint32_t nrl_max;
  uint32_t nrl_count;
  uint32_t nrl_first;
  uint32_t nrl_last;
  uint32_t nrl_free;
  nx_reap_list_entry_t nrl_entries[];
};

struct nx_reap_list_entry {
  uint32_t nrle_next;
  uint32_t nrle_flags;
  uint32_t nrle_type;
  uint32_t nrle_size;
  oid_t nrle_fs_oid;
  oid_t nrle_oid;
  xid_t nrle_xid;
};

struct omap_reap_state {
  uint32_t omr_phase;
  omap_key_t omr_ok;
};

struct omap_cleanup_state {
  uint32_t omc_cleaning;
  uint32_t omc_omsflags;
  xid_t omc_sxidprev;
  xid_t omc_sxidstart;
  xid_t omc_sxidend;
  xid_t omc_sxidnext;
  omap_key_t omc_curkey;
};

struct apfs_reap_state {
  uint64_t last_pbn;
  xid_t cur_snap_xid;
  uint32_t phase;
}
__attribute__((packed));

struct er_state_phys {
  er_state_phys_header_t ersb_header;
  uint64_t ersb_flags;
  uint64_t ersb_snap_xid;
  uint64_t ersb_current_fext_obj_id;
  uint64_t ersb_file_offset;
  uint64_t ersb_progress;
  uint64_t ersb_total_blk_to_encrypt;
  oid_t ersb_blockmap_oid;
  uint64_t ersb_tidemark_obj_id;
  uint64_t ersb_recovery_extents_count;
  oid_t ersb_recovery_list_oid;
  uint64_t ersb_recovery_length;
};

struct er_state_phys_v1 {
  er_state_phys_header_t ersb_header;
  uint64_t ersb_flags;
  uint64_t ersb_snap_xid;
  uint64_t ersb_current_fext_obj_id;
  uint64_t ersb_file_offset;
  uint64_t ersb_fext_pbn;
  uint64_t ersb_paddr;
  uint64_t ersb_progress;
  uint64_t ersb_total_blk_to_encrypt;
  uint64_t ersb_blockmap_oid;
  uint32_t ersb_checksum_count;
  uint32_t ersb_reserved;
  uint64_t ersb_fext_cid;
  uint8_t ersb_checksum[0];
};

struct er_state_phys_header {};

struct er_recovery_block_phys {
  obj_phys_t erb_o;
  uint64_t erb_offset;
  oid_t erb_next_oid;
  uint8_t erb_data[0];
};

struct gbitmap_block_phys {
  obj_phys_t bmb_o;
  uint64_t bmb_field[0];
};

struct gbitmap_phys {
  obj_phys_t bm_o;
  oid_t bm_tree_oid;
  uint64_t bm_bit_count;
  uint64_t bm_flags;
};

typedef struct {
  obj_phys_t fwp_objHdr;
  uint64_t fwp_version;
  oid_t fwp_listHeadOid;
  oid_t fwp_listTailOid;
  uint64_t fwp_stableHeadOffset;
  uint64_t fwp_stableTailOffset;
  uint32_t fwp_listBlocksCount;
  uint32_t fwp_reserved;
  uint64_t fwp_usedByRC;
  prange_t fwp_rcStash;
}
fusion_wbc_phys_t;

typedef struct {
  paddr_t fwle_wbcLba;
  paddr_t fwle_targetLba;
  uint64_t fwle_length;
}
fusion_wbc_list_entry_t;

typedef struct {
  obj_phys_t fwlp_objHdr;
  uint64_t fwlp_version;
  uint64_t fwlp_tailOffset;
  uint32_t fwlp_indexBegin;
  uint32_t fwlp_indexEnd;
  uint32_t fwlp_indexMax;
  uint32_t fwlp_reserved;
  fusion_wbc_list_entry_t fwlp_listEntries[];
}
fusion_wbc_list_phys_t;

typedef struct {
  paddr_t fmv_lba;
  uint32_t fmv_length;
  uint32_t fmv_flags;
}
fusion_mt_val_t;
