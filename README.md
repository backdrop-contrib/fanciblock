Fanciblock
==========

DESCRIPTION
-----------
Fanciblock module can be used to create image and icon feature boxes.

INSTALLATION
------------
1. Copy fanciblock directory to your modules directory
2. Enable the module at module administration page
3. Add a Fanciblock in Layout Builder. Two types of Fanciblocks are available:
 - Fanciblock image: provides feature blocks using uploaded images.
 - Fanciblock icon: provides feature blocks with fontawesome icon fonts.
4. Configure the block settings. On save, a default image (or icon) and text 
   be used to create the blocks. 
5. Navigate to a path where your chosen layout is active to view the Fanciblock.
6. Use the contextual links menu to edit individual block boxes.
 - Box icons can be selected from the select list
 - Box images can be uploaded or changed
 - Icon colors can be edited individually
 - Box content and titles can be edited
7. Block boxes are listed and can be re-ordered at 'admin/config/media/fanciblocks/%'
8. A list of all Fanciblocks can be found at 'admin/config/media/fanciblocks/'
 - IMPORTANT! Because of the way Layout builder saves blocks, if you configure
   a new block, then click "Add Block" but then do not save the layout, the 
   block settings are still saved in fancibock.settings.json, but the block will
   not be added to the layout.
   Until a hook_layout_save() or equivalent lands in core, this will be so.
   Unused settings are, however cleared by cron, and can be cleared immediately
   by running cron.

That's it!

Screenshots
-----------

![image1](/../image-branch/screenshots/capture.png?raw=true "")

![image2](/../image-branch/screenshots/capture2.png?raw=true "")

![image3](/../image-branch/screenshots/capture3.png?raw=true "")

LICENSE
---------------    

This project is GPL v2 software. See the LICENSE.txt file in this directory 
for complete text.

CURRENT MAINTAINERS
---------------    

- Andy Shillingford (https://github.com/docwilmot/)

CREDITS   
--------------- 

Originally ported from Iconbox module by developmenticon.com, backdropchamp.com
