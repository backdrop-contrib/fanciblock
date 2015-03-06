<?php
/**
 * @file
 * Default output for a fanciblock.
*/
?>
<div class="fanciblock-wrapper">
  <div class="fanciblock" id="fanciblock-<?php print $id; ?>">
    <div class="fanciblock-grid fanciblock-grid-<?php print $grid; ?>"><div class="fanciblock-grid-inner">
      <?php foreach($items as $item) : ?>
        <div <?php print backdrop_attributes(array('class' => $col_class)) ?>><div class="fanciblock-col-inner">
          <div class="fanciblock-box <?php print $box_class; ?>" <?php print $box_style; ?>>
            <?php if($icon_position == 'fanciblock-icon-position-left'): ?>
              <div class="fanciblock-icon"><?php print $item['image'] ?></div>
            <?php endif; ?>
            <div class="fanciblock-title-content">
              <h3 class="fanciblock-title"><?php print render($item['box_title']); ?></h3>
              <div class="fanciblock-content"><?php print render($item['box_content']); ?></div>
            </div>
            <?php if($icon_position == 'fanciblock-icon-position-right'): ?>
              <div class="fanciblock-icon"><?php print $item['image'] ?></div>
            <?php endif; ?>
          </div>
        </div></div>
      <?php endforeach; ?>
      <div class="clear"></div>
    </div></div>
  </div>
</div>