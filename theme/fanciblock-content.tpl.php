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
        <?php // Add first, last, odd, even class
          $i++;
          $col_class[0] = 'fanciblock-col';
          $col_class[1] = 'fanciblock-col-'.$i;
          if ($i == 1) { $col_class[2] = 'col-first';}
          if ($i == count($items)) {$col_class[2] = 'col-last';}
          if ($i % 2 == 0) {$col_class[4] = 'col-even';}
          if ($i % 2 != 0) {$col_class[4] = 'col-odd';}
        ?>

        <div <?php print backdrop_attributes(array('class' => $col_class)) ?>><div class="fanciblock-col-inner">
          <div class="fanciblock-box <?php print $icon_position.' '.$box_padding; ?>" <?php print $box_style; ?>>
            <div class="fanciblock-icon"><?php print theme('fanciblock_image_fontawesome_icon_build', array('class' => $item['fontawesome_icon'], 'size' => $icon_size, 'looks' => $icon_looks, 'style' => $icon_style)); ?></div>
            <div class="fanciblock-title-content">
              <h3 class="fanciblock-title"><?php print render($item['box_title']); ?></h3>
              <div class="fanciblock-content"><?php print render($item['box_content']); ?></div>
            </div>
          </div>
        </div></div>
      <?php endforeach; ?>
      <div class="clear"></div>
    </div></div>
  </div>
</div>