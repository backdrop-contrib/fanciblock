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
function template_preprocess_fanciblock_content(&$variables) {
  // Each fanciblock instance gets a unique id
  $fanciblock_id = &backdrop_static('fanciblock_id', 0);
  $variables['id'] = ++$fanciblock_id;
  $variables['i']  = 0;

  // Make all variables available.
  $variables['box_padding'] = NULL;
  $variables['icon_size'] = NULL;
  $variables['icon_style'] = NULL;
  $box_border = NULL;
  
  $variables['icon_bg_color'] = NULL;
  $variables['icon_bg_color_status'] = NULL;

  $variables['icon_border_status'] = NULL;
  $variables['icon_border'] = NULL;
  $variables['icon_border_width'] = NULL;
  $variables['icon_border_style'] = NULL;
  $variables['icon_border_color'] = NULL;

  $variables['box_bg_color_status'] = NULL;
  $variables['box_bg_color'] = NULL;

  $variables['box_border_status'] = NULL;
  $variables['box_border'] = NULL;
  $variables['box_border_width'] = NULL;
  $variables['box_border_style'] = NULL;
  $variables['box_border_color'] = NULL;

  // define width, height, bgcolor, bgimage for fanciblock
  if(!empty($variables['settings'])) {
    $variables['fanciblock_type'] = $variables['settings']['fanciblock_type'];

    $variables['grid']				= $variables['settings']['grid'];
    $variables['icon_position']		= 'fanciblock-icon_position-'.$variables['settings']['icon_position'];
    $variables['icon_looks']			= 'fanciblock-icon_style-'.$variables['settings']['icon_style'];
    $variables['icon_size']			= 'fanciblock-icon_size-'.$variables['settings']['icon_size'];
    
    // Build icon border and background color
    if($variables['settings']['icon_style']	!= 'none') {
      // Build background color style.
      if($variables['settings']['icon_bg_color_status']	== '') {
        $variables['icon_bg_color']	= 'background-color:transparent;';
      } else {
        $variables['icon_bg_color']	= 'background-color:'.$variables['settings']['icon_bg_color'].';';
      }
      
      // If icon border is set, then build the whole sentence by joining the 
      // border width, style and color settings sentences.
      if($variables['settings']['icon_border_status']	== 1) {
        $variables['icon_border']	= 'border-width:';
        $variables['icon_border']	.= $variables['settings']['icon_border_width'] .'px;';
        $variables['icon_border']	.= 'border-style:';
        $variables['icon_border']	.= $variables['settings']['icon_border_style'].';';
        $variables['icon_border']	.= 'border-color:';
        $variables['icon_border']	.= $variables['settings']['icon_border_color'].';';
      }
    
    }
    // Create icon style sentence by joining icon color and background border.
    $variables['icon_style']	= $variables['icon_bg_color'] . $variables['icon_border']; 

    // Build the box background.
    if($variables['settings']['box_bg_color_status'] == '') {
      $box_bg_color	= 'background-color:transparent;';
    } else {
      $box_bg_color	= 'background-color:'.$variables['settings']['box_bg_color'].';';
    }
    
    // Build the box border.
    if($variables['settings']['box_border_status'] == 1) {
      // Build a border color sentence.
      $box_border	= 'border-width:';
      $box_border	.= $variables['settings']['box_border_width'] .'px;';
      $box_border	.= 'border-style:';
      $box_border	.= $variables['settings']['box_border_style'].';';
      $box_border	.= 'border-color:';
      $box_border	.= $variables['settings']['box_border_color'].';';
    }
    
    // Build padding
    $box_padding		= 'padding:' . $variables['settings']['box_padding'] . 'px;';
    
    // Finally
    $variables['box_style'] = 'style ="';
    $variables['box_style'] .= $box_bg_color;
    $variables['box_style'] .= $box_padding;
    $variables['box_style'] .= $box_border;
    $variables['box_style'] .= '"';
    
    $variables['box_class'] = $variables['icon_position'] .' ' . $variables['icon_looks'] . ' ' . $variables['icon_size'];
  }
  
  //
  $variables['items'] = array();
  $i = 0;
  $item_number = count($variables['fanciblock_data']);
  foreach ($variables['fanciblock_data'] as $box => $item ) {
    // Build column classes.
    // Add first, last, odd, even class.
    $i++;
    $variables['col_class'][0] = 'fanciblock-grid-'.$i;
    $variables['col_class'][1] = 'fanciblock-col';
    if ($i == 1) { $variables['col_class'][2] = 'col-first';}
    if ($i == $item_number) {$variables['col_class'][2] = 'col-last';}
    if ($i % 2 == 0) {$variables['col_class'][4] = 'col-even';}
    if ($i % 2 != 0) {$variables['col_class'][4] = 'col-odd';}
    
    // Add icon link
    if(!empty($item['icon_link'])) {
      //$variables['icon_link'] = $item['icon_link'];
    }
    
    // Build the images or icons.
    if($variables['fanciblock_type'] == 'fanciblock') {
      // Use the full uploaded image for 'fullwidth' and 'none' styles.
      if($variables['settings']['icon_size'] == 'none' || $variables['settings']['icon_size'] == 'fullwidth') {
        $item['image'] = theme('image', array('uri' => $item['fanciblock_uri']));      
      }
      else {
        $item['image'] = theme('image_style', array('uri' => $item['fanciblock_uri'], 'style_name' => $variables['settings']['icon_size']));
      }
    }
    else {
      $item['image'] = theme('fanciblock_fontawesome_icon_build', array(
        'class' => $item['fontawesome_icon'], 
        'size' => $variables['icon_size'], 
        'looks' => $variables['icon_looks'], 
        'style' => $variables['icon_style'] . 'color:' . $item['fontawesome_icon_color'],
      ));
    }
    
    $item['image'] = (empty($item['icon_link'])) ? $item['image'] : l($item['image'], $item['icon_link'], array('html' => TRUE));
    $item['box_content'] = filter_xss($item['box_content']['value']);

    $variables['items'][$box] = $item;
  }
}

/**
 * Template preprocess handler for 'fanciblock_content' theme.
 */
function theme_fanciblock_box_preview(&$variables) {
  backdrop_add_css(backdrop_get_path('module', 'fanciblock') . '/fontawesome/css/font-awesome.css'); // add library fontawesome.css
  backdrop_add_css(backdrop_get_path('module', 'fanciblock') . '/css/fanciblock.css'); // add css
  backdrop_add_js(backdrop_get_path('module', 'fanciblock') . '/js/fanciblock.js'); // add js

  $form = $variables['form'];
  $fanciblock = $form['#fanciblock'];
  unset($form['fanciblock']);  
  
  $output = '';
  $box = $form['#box'];

  if($fanciblock['fanciblock_type'] == 'fanciblock') {
      $image = theme('image_style', array('uri' => $box['fanciblock_uri'], 'style_name' => 'thumbnail'));
  }
  else {
    $icon_style	= 'style="'. $fanciblock['icon_bg_color'] . $fanciblock['icon_border']; 

    $image = theme('fanciblock_fontawesome_icon_build', array(
      'class' => $box['fontawesome_icon'], 
      'size' => 'small', 
      'looks' => '', 
      'style' => 'style="color:' . $box['fontawesome_icon_color'] . ';"',
    ));
  }

  $output .= '<div class="fanciblock-box-wrapper">';
  $output .= '<div class="fanciblock-col-inner">';
    $output .= '<div class="fanciblock-box">';
      if ($fanciblock['icon_position'] == 'left' || $fanciblock['icon_position'] == 'top') {
        $output .= '<div class="fanciblock-icon">';
        $output .= $image;
        $output .= '</div>';
      }
      $output .= '<div class="fanciblock-title-content">';
      $output .= $box['box_title'];
      $output .= '</div>';
      $output .= '<div class="fanciblock-button">';
      $output .= $fanciblock['disabled'] ? t('Edit') : backdrop_render($form['button']);
      $output .= '</div>';
      $output .= '<div class="fanciblock-button">';
      $output .= $fanciblock['disabled'] ? t('Delete') : backdrop_render($form['delete']);
      $output .= '</div>';
      if ($icon_position == 'fanciblock-icon_position-right') {
        $output .= '<div class="fanciblock-icon">';
        $output .= $image;
        $output .= '</div>';
      }
    $output .= '</div>';
  $output .= '</div>';
  $output .= '</div>';

  return $output;
}

/**
 * Returns HTML for a fanciblock title and description for the fanciblock overview page.
 *
 * @param $variables
 *   An associative array containing:
 *   - title: The fanciblock's title.
 *
 * @ingroup themeable
 */
function theme_fanciblock_fontawesome_icon_build($variables) {
  $options = array('attributes' => array(
    'style' => $variables['style'], 
    'class' => array(
      $variables['size'], 
      $variables['looks']
  )));

  return icon($variables['class'], $options);
}
