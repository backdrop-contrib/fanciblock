<?php
/**
 * @file
 * Theming functions for the fanciblock module.
 * 
 * Preprocessor functions fill variables for templates and helper
 * functions to make theming easier.
 */

/**
 * Template preprocess handler for 'fanciblock_content' theme.
 */
function template_preprocess_fanciblock_content(&$vars) {
  // Each fanciblock instance gets a unique id
  dpm($vars);
  $fanciblock_id = &backdrop_static('fanciblock_id', 0);
  $vars['id'] = ++$fanciblock_id;
  $vars['i']  = 0;

  $vars['icon_size']	= NULL;
  $vars['icon_color']		= NULL;
  $vars['icon_bg_color']	= NULL;
  $vars['icon_border']		= NULL;
  $vars['icon_border_color']= NULL;
  $vars['icon_bg_border']	= NULL;
  $vars['box_bg_color']		= NULL;
  $vars['box_border_color']	= NULL;
  $vars['box_border']		= NULL;
  $vars['box_padding']		= NULL;

  // define width, height, bgcolor, bgimage for fanciblock
  if(!empty($vars['settings'])) {
    // Todo, this is ugly; add block type to settings array.
    $vars['fanciblock_type'] = $vars['settings']['fanciblock_type'];

    $vars['grid']				= $vars['settings']['grid'];
    $vars['icon_position']		= 'fanciblock-icon-position-'.$vars['settings']['icon-position'];
    $vars['icon_looks']			= 'fanciblock-icon-style-'.$vars['settings']['icon-style'];
    $vars['icon_size']			= 'fanciblock-icon-size-'.$vars['settings']['icon-size'];
    
    // Build background color style sentence if settings allow, otherwise set as
    // transparent.
    if($vars['settings']['icon-bg-color']		== '')		{$vars['icon_bg_color']		= 'background-color:transparent;';} else
    	{$vars['icon_bg_color']		= 'background-color:'.$vars['settings']['icon-bg-color'].';';}
    
    // Build border-color sentence
    $vars['icon_border_color']	= 'border-color:'.$vars['settings']['icon-border-color'].';';
    
    // If icon border is set, then build the whole sentence by joining the 
    // border width, style and color settings sentences.
    if($vars['settings']['icon-border']			== 1) 			{$vars['icon_border']		= 'border-width:'.$vars['settings']['icon-border-width'].'px;border-style:'.$vars['settings']['icon-border-style'].';'.$vars['icon_border_color'];}
    
    // Join icon border and background color
    if($vars['settings']['icon-style']			!= 'none')		{$vars['icon_bg_border']	= $vars['icon_bg_color'] . $vars['icon_border']; }

    // Create icon style sentence by joining icon color and background border.
    $vars['icon_style']							= 'style=' . $vars['icon_bg_border']; // icon styles

    // Build the box settings: set the bg color to transparent if the setting is
    // off.
    if($vars['settings']['box-bg-color']		== '')		{$vars['box_bg_color']		= 'background-color:transparent;';} else 
      {$vars['box_bg_color']		= 'background-color:'.$vars['settings']['box-bg-color'].';';}
    
    // Build a border color sentence.
    $vars['box_border_color']	= 'border-color:'.$vars['settings']['box-border-color'].';';
    
    // Build the whole box border now, mashing width, style and color.
    if($vars['settings']['box-border']			== 1) 			{$vars['box_border']		= 'border-width:'.$vars['settings']['box-border-width'].'px;border-style:'.$vars['settings']['box-border-style'].';'.$vars['box_border_color'];}
    
    // Build padding
    $box_padding		= ';padding:' . $vars['settings']['box-padding'] . 'px; ';
    
    // Build the box style: mash bg color, padding and border.
    $vars['box_style']							= 'style='.$vars['box_bg_color'] . $box_padding . $vars['box_border']; // box styles
    
    if($vars['settings']['box-border']			== 1)			{$vars['box_padding']		= 'fanciblock-padding';}
    
    $vars['box_class'] = $vars['icon_position'] .' ' . $vars['icon_looks'] . ' ' . $vars['icon_size'];
  }

  //
  $vars['items'] = array();
  $i = 0;
  $item_number = count($vars['fanciblock_data']);
  foreach ($vars['fanciblock_data'] as $box => $item ) {
    // Add first, last, odd, even class
    $i++;
    $vars['col_class'][0] = 'fanciblock-col';
    $vars['col_class'][1] = 'fanciblock-col-'.$i;
    if ($i == 1) { $vars['col_class'][2] = 'col-first';}
    if ($i == $item_number) {$vars['col_class'][2] = 'col-last';}
    if ($i % 2 == 0) {$vars['col_class'][4] = 'col-even';}
    if ($i % 2 != 0) {$vars['col_class'][4] = 'col-odd';}
    
    if($vars['fanciblock_type'] == 'fanciblock') {
      // Use the full uploaded image for 'fullwidth' and 'none' styles.
      if($vars['settings']['icon-size'] == 'none' || $vars['settings']['icon-size'] == 'fullwidth') {
        $item['image'] = theme('image', array('uri' => $item['fanciblock_uri']));      
      }
      else {
        $item['image'] = theme('image_style', array('uri' => $item['fanciblock_uri'], 'style_name' => $vars['settings']['icon-size']));
      }
    }
    else {
      $item['image'] = theme('fanciblock_fontawesome_icon_build', array(
        'class' => $item['fontawesome_icon'], 
        'size' => $vars['icon_size'], 
        'looks' => $vars['icon_looks'], 
        'style' => $vars['icon_style'],
        'color' => 'color:'.$item['fontawesome_icon_color']
      ));
    }
    // Add boxes
    $vars['items'][$box] = $item;
  }

}