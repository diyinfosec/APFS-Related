

## Create a disk image 
* Disk images are data containers that emulate disks.  Like disks, disk images can also be partitioned and formatted.
* For understanding APFS creating disk images is a flexible option. 
* The hdiutil command can be used to create/manipulate disk images that have APFS containers inside them. 
* Extra: hdiutil uses the DiskImages framework to manipulate disk images. 

```
hdiutil create -fs APFS -size 200MB 200mb.apfs
created: /private/tmp/200mb.apfs.dmg
```

## Locate the APFS container inside the image
* When you run the `hdiutil create` command it will create a disk image. 
* Inside the disk image you will find an APFS container (because we specified `-fs APFS` option). 
* Since our goal is to study APFS data structures, we must locate the APFS container inside the disk image. 
* This can be done using `hdiutil imageinfo` command or the `mmls` command. 
* Note: If you run the `hdiutil` after attaching the image you will get a `hdiutil: imageinfo failed - Resource temporarily unavailable` error. `mmls` can however be used irrespective of whether the image is attached or detached. 

### Approach 1 - Using hdiutil
* The hdiutil output is bit verbose. I am giving a snipped version of the output. 
* Under the `partitions` section of the output, look at item #4 for details of the APFS container. 
* The `partition-start` will be a multiple of the sector size (512). The `partition-filesystems` will give the list of volumes under that APFS container. 
```
hdiutil imageinfo 200mb.apfs.dmg 
Backing Store Information:
	URL: file:///private/tmp/200mb.apfs.dmg
	Name: 200mb.apfs.dmg
	Class Name: CBSDBackingStore
Class Name: CRawDiskImage
Checksum Type: none
Size Information:
	Total Bytes: 209715200
	Compressed Ratio: 1
	Sector Count: 409600
	Total Non-Empty Bytes: 209715200
	Compressed Bytes: 209715200
	Total Empty Bytes: 0
Format: UDRW
Format Description: raw read/write
Checksum Value: 
Properties:
	Encrypted: false
	Kernel Compatible: true
	Checksummed: false
	Software License Agreement: false
	Partitioned: false
	Compressed: no
Segments:
	0: /private/tmp/200mb.apfs.dmg
partitions:
	partition-scheme: GUID
	block-size: 512
	partitions:
		0:
			partition-name: Protective Master Boot Record
			partition-start: 0
			partition-synthesized: true
			partition-length: 1
			partition-hint: MBR
		1:
			partition-name: GPT Header
			partition-start: 1
			partition-synthesized: true
			partition-length: 1
			partition-hint: Primary GPT Header
		2:
			partition-name: GPT Partition Data
			partition-start: 2
			partition-synthesized: true
			partition-length: 32
			partition-hint: Primary GPT Table
		3:
			partition-name: 
			partition-start: 34
			partition-synthesized: true
			partition-length: 6
			partition-hint: Apple_Free
		4:
			partition-UUID: E9F72AC6-59D3-43B1-AAC0-216E744CCD30
			partition-name: disk image
			partition-hint-UUID: 7C3457EF-0000-11AA-AA11-00306543ECAC
			partition-start: 40
			partition-number: 1
			partition-length: 409520
			partition-hint: Apple_APFS
			partition-filesystems:
				APFS: Untitled
		5:
			partition-name: 
			partition-start: 409560
			partition-synthesized: true
			partition-length: 7
			partition-hint: Apple_Free
		6:
			partition-name: GPT Partition Data
			partition-start: 409567
			partition-synthesized: true
			partition-length: 32
			partition-hint: Backup GPT Table
		7:
			partition-name: GPT Header
			partition-start: 409599
			partition-synthesized: true
			partition-length: 1
			partition-hint: Backup GPT Header
	burnable: false
Resize limits (per hdiutil resize -limits):
 min 	 cur 	 max 
409600	409600	267442120
```

### Approach 2 - Using mmls from the Sleuthkit toolset. 
* The mmls output is given below. Look at row "004" with the description **disk image**. This is located at an offset of 40 from the beginning of the image. 
* The offset is given in units of 512-byte sectors (also shown in the output). So the actual offset is 40*512=**20480**. 

```
mmls ./200mb.apfs.dmg
GUID Partition Table (EFI)
Offset Sector: 0
Units are in 512-byte sectors

      Slot      Start        End          Length       Description
000:  Meta      0000000000   0000000000   0000000001   Safety Table
001:  -------   0000000000   0000000039   0000000040   Unallocated
002:  Meta      0000000001   0000000001   0000000001   GPT Header
003:  Meta      0000000002   0000000033   0000000032   Partition Table
004:  000       0000000040   0000409559   0000409520   disk image
005:  -------   0000409560   0000409599   0000000040   Unallocated
```

## Attach the image 

* Attaching means connecting the disk image to the system. 
* Attaching an image will also automatically mount the APFS container and the volumes inside the container. 
* Once a disk image is attached we can create files/directories inside it. 


```
hdiutil attach 200mb.apfs.dmg 
/dev/disk3          	GUID_partition_scheme          	
/dev/disk3s1        	Apple_APFS                     	
/dev/disk4          	EF57347C-0000-11AA-AA11-0030654	
/dev/disk4s1        	41504653-0000-11AA-AA11-0030654	/Volumes/untitled
```

As you can see in the above output:
* When attaching the image it has created two disks: _disk3_ and _disk4_. 
* _**disk3**_ represents the image (dmg) that we created. 
* _**disk4**_ is a "synthesized" disk aka the APFS container. 
* _**/dev/disk4s1**_ is a volume that has been automatically created under the APFS container (_disk4_). This volume is mounted under _**/Volumes/untitled**_

## Ejecting the disk
* Ejecting the disk will basically _detach_ it from the system. 
* You can use either `hdiutil` or `diskutil` to eject. 

```
hdiutil eject disk3
"disk3" ejected.
```
(or) 
```
diskutil eject disk3
Disk disk3 ejected
```

For some reason, unmountDisk is not working, even though it says successful. 
```
diskutil unmountDisk disk4
Unmount of all volumes on disk4 was successful
```


## Additional notes
* After running , a disk image (dmg) becomes a disk. You can use the diskutil commands with disk3/disk4. Example:  
```
diskutil list disk3
/dev/disk3 (disk image):
   #:                       TYPE NAME                    SIZE       IDENTIFIER
   0:      GUID_partition_scheme                        +209.7 MB   disk3
   1:                 Apple_APFS Container disk4         209.7 MB   disk3s1



diskutil list disk4
/dev/disk4 (synthesized):
   #:                       TYPE NAME                    SIZE       IDENTIFIER
   0:      APFS Container Scheme -                      +209.7 MB   disk4
                                 Physical Store disk3s1
   1:                APFS Volume untitled                24.6 KB    disk4s1
```
* If multiple disk images get mounted as disks, how will you know which of these is the physical disk and which of these is virtual? You can use `diskutil list physical` - for listing physical disks and `diskutil list virtual` - for listing virtual disks. 

* If you want APFS specific information for a disk you can use `diskutil apfs list <diskname>`. Note: This will work only for synthesized disks that are APFS containers. 
```
diskutil apfs list disk3
disk3 is not an APFS Container


diskutil apfs list disk4
|
+-- Container disk4 4820F4FD-366A-43E0-AA67-C123C7AD3FA6
    ====================================================
    APFS Container Reference:     disk4
    Size (Capacity Ceiling):      209674240 B (209.7 MB)
    Capacity In Use By Volumes:   1486848 B (1.5 MB) (0.7% used)
    Capacity Not Allocated:       208187392 B (208.2 MB) (99.3% free)
    |
    +-< Physical Store disk3s1 E9F72AC6-59D3-43B1-AAC0-216E744CCD30
    |   -----------------------------------------------------------
    |   APFS Physical Store Disk:   disk3s1
    |   Size:                       209674240 B (209.7 MB)
    |
    +-> Volume disk4s1 F2CD8F43-1651-4AC3-B0A3-9D681EF68123
        ---------------------------------------------------
        APFS Volume Disk (Role):   disk4s1 (No specific role)
        Name:                      untitled (Case-insensitive)
        Mount Point:               /Volumes/untitled
        Capacity Consumed:         24576 B (24.6 KB)
        FileVault:                 No
```



```
file 200mb.apfs.dmg 
200mb.apfs.dmg: DOS/MBR boot sector; partition 1 : ID=0xee, start-CHS (0x3ff,254,63), end-CHS (0x3ff,254,63), startsector 1, 409599 sectors, extended partition table (last)
```
