<?php
/**
 * @file
 * Administrative page callbacks for fanciblock module.
 */

/**
 * Menu callback which shows an overview page of all the fanciblocks.
 */
function fanciblock_overview_page() {
  $result = config_get('fanciblock.settings');
  
  if($result) {
    $header = array(t('Fanciblock name'), 'data' => t('Operations'));
    $rows = array();
    foreach ($result as $key => $fanciblock) {
      if(is_array($fanciblock)) {
        $row = array();
        $row[] = check_plain($key);
        $row[] = array('data' => l(t('edit'), 'admin/config/media/fanciblocks/' . $key));
        $rows[] = $row;
      }
    }
  }
  else {
    $header = array(t('fanciblock Name'), array('data' => t('Operations')));
    $rows = array();
    $rows[] = array(array('data' => t('There are no fanciblock yet')));
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
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
function theme_fanciblock_admin_overview($variables) {
  $output = check_plain($variables['title']);
  return $output;
}

/**
 * Menu callback; Build the form that handles the adding/editing of a fanciblock.
 */
function fanciblock_edit_box($type, $delta, $fanciblock = array()) {
  backdrop_add_css(backdrop_get_path('module', 'fanciblock') . '/css/fanciblock.admin.css');
  dvm($fanciblock, 'feb');

  $grid_count = isset($fanciblock['grid']) ? $fanciblock['grid'] : 3;
  $fanciblock += array(
    'fanciblock_name' => '',
    'title' => '',
    'optionset' => '',
  );
  $form['fanciblock_type'] = array(
    '#type' => 'hidden',
    '#value' => $delta,
  );

  $form['fanciblock_title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Fanciblock Title'),
    '#description' => t('A unique name for this Fanciblock.'),
    '#default_value' => isset($fanciblock['fanciblock_title']) ? $fanciblock['fanciblock_title'] : '',
  );
  $form['fanciblock_name'] = array(
    '#required' => TRUE,
    '#type' => 'machine_name',
    '#title' => t('Fanciblock ID'),
    '#default_value' => isset($fanciblock['fanciblock_name']) ? $fanciblock['fanciblock_name'] : '',
    '#description' => t('A unique name for this Fanciblock. It must only contain lowercase letters, numbers and underscores.'),
    '#machine_name' => array(
      'exists' => 'fanciblock_load',
      'source' => array('block_settings', 'fanciblock_title'),
    ),
  );

  // Appearance fanciblock
  $form['appearance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Appearance'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['appearance']['icon_position'] = array(
    '#type' => 'select',
    '#title' => t('Icon Position'),
    '#options' => array(
      "top"  => t('Top'),
      "left" => t('Left'),
      "right"  => t('Right'),
      "boxcut" => t('Boxcut'),
    ),
    '#default_value' => isset($fanciblock['icon_position']) ? $fanciblock['icon_position'] : 'top',
  );
  $form['appearance']['icon_style'] = array(
    '#type' => 'select',
    '#title' => t('Icon Style'),
    '#options' => array(
      "none" => t('None'),
      "circle" => t('Circle'),
      "square"  => t('Square'),
    ),
    '#default_value' => isset($fanciblock['icon_style']) ? $fanciblock['icon_style'] : 'none',
  );
  
  if($delta == 'fanciblock') {
  /*----------- icon size -----------*/
    $form['appearance']['icon_size'] = array(
      '#type' => 'select',
      '#title' => t('Icon Size'),
      '#options' => array(
        "none"  => t('No style'),
        "fullwidth"  => t('Full width'),
        "medium"  => t('Very large (220x220)'),
        "thumbnail"  => t('Large (100x100)'),
        "small_75x75" => t('Medium (75x75)'),
        "tiny_40x40"  => t('Small (40x40)'),
      ),
      '#default_value' => isset($fanciblock['icon_size']) ? $fanciblock['icon_size'] : 'medium',
    );
  /*----------- end icon size -----------*/
  }
  elseif($delta == 'fanciicon') {
  /*----------- icon size -----------*/
    $form['appearance']['icon_size'] = array(
      '#type' => 'select',
      '#title' => t('Icon Size'),
      '#options' => array(
        "xlarge"  => t('Extra Large (150x150)'),
        "large"  => t('Large (110x110)'),
        "medium" => t('Medium (70x70)'),
        "small"  => t('Small (45x45)'),
        "tiny"  => t('Tiny (25x25)'),
      ),
      '#default_value' => isset($fanciblock['icon_size']) ? $fanciblock['icon_size'] : 'medium',
    );
  /*----------- end icon size -----------*/
  }
/*----------- Box padding -----------*/
  $form['appearance']['box_padding'] = array(
    '#type' => 'textfield',
    '#title' => t('Box Padding'),
    '#size' => 10,
    '#maxlength' => 2,
    '#default_value' => isset($fanciblock['box_padding']) ? $fanciblock['box_padding'] : '20',
  );
/*----------- end Box padding -----------*/

/*----------- icon bg color -----------*/
  $form['appearance']['icon_bg_color_wrapper'] = array(
    '#type' => 'container',
    '#title' => t('Icon Background Color'),
    '#states' => array(
      'invisible' => array(
        ':input[name="block_settings[appearance][icon_style]"]' => array('value' => 'none'),
      ),
    ),
  );
  $form['appearance']['icon_bg_color_wrapper']['icon_bg_color_status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Icon Background Color'),
    '#default_value' => isset($fanciblock['icon_bg_color_status']) ? $fanciblock['icon_bg_color_status'] : false,
  );
  $form['appearance']['icon_bg_color_wrapper']['icon_bg_color_container'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Icon Background Color'), 
    '#title_display' => 'invisible',
    '#attributes' => array('class' => array('container-inline')),
    '#states' => array(
      'visible' => array(
        ':input[name="block_settings[appearance][icon_bg_color_wrapper][icon_bg_color_status]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['appearance']['icon_bg_color_wrapper']['icon_bg_color_container']['icon_bg_color'] = array(
    '#type' => 'color',
    '#title' => t('Icon Background Color'),
    '#description' => t('Enter background color'),
    '#element_validate' => array('_fanciblock_validate_color_value'),
    '#default_value' => isset($fanciblock['icon_bg_color']) ? $fanciblock['icon_bg_color'] : '#333333',
  );
/*----------- end icon bg color -----------*/

/*----------- icon border -----------*/
  $form['appearance']['icon_border_wrapper'] = array(
    '#type' => 'container',
    '#title' => t('Icon Background Color'),
    '#states' => array(
      'invisible' => array(
        ':input[name="block_settings[appearance][icon_style]"]' => array('value' => 'none'),
      ),
    ),
  );
  $form['appearance']['icon_border_wrapper']['icon_border_status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Icon Border'),
    '#default_value' => isset($fanciblock['icon_border']) ? $fanciblock['icon_border'] : false,
  );
  $form['appearance']['icon_border_wrapper']['icon_border_container'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Icon Border Settings'), 
    '#title_display' => 'invisible',
    '#attributes' => array('class' => array('container-inline')),
    '#states' => array(
      'visible' => array(
        ':input[name="block_settings[appearance][icon_border_wrapper][icon_border_status]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['appearance']['icon_border_wrapper']['icon_border_container']['icon_border_width'] = array(
    '#type' => 'select',
    '#title' => t('Icon Border Width'),
    '#title_display' => 'invisible',
    '#options' => array('1' => 1, 2, 3, 4, 5, 6, 7, 8),
    '#default_value' => isset($fanciblock['icon_border_width']) ? $fanciblock['icon_border_width'] : 1,
  );
  $form['appearance']['icon_border_wrapper']['icon_border_container']['icon_border_style'] = array(
    '#type' => 'select',
    '#title' => t('Icon Border Style'),
    '#title_display' => 'invisible',
    '#options' => array(
      "solid"	=> t('Solid'),
      "dashed"	=> t('Dashed'),
      "dotted"	=> t('Dotted'),
      "double"	=> t('Double'),
    ),
    '#default_value' => isset($fanciblock['icon_border_style']) ? $fanciblock['icon_border_style'] : 1,
  );
  $form['appearance']['icon_border_wrapper']['icon_border_container']['icon_border_color'] = array(
    '#type' => 'color',
    '#title' => t('Icon Custom Border Color'),
    '#title_display' => 'invisible',
    '#description' => t('Enter background color'),
    '#element_validate' => array('_fanciblock_validate_color_value'),
    '#default_value' => isset($fanciblock['icon_border_color']) ? $fanciblock['icon_border_color'] : '#efefef',
  );
/*----------- end icon border -----------*/


/*----------- Box background -----------*/
  $form['appearance']['box_bg_color_status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Box Background Color'),
    '#default_value' => isset($fanciblock['box_bg_color_status']) ? $fanciblock['box_bg_color_status'] : false,
  );
  $form['appearance']['box_bg_color_container'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Box Background Color'),
    '#title_display' => 'invisible',
    '#attributes' => array('class' => array('container-inline')),
    '#states' => array(
      'visible' => array(
        ':input[name="block_settings[appearance][box_bg_color_status]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['appearance']['box_bg_color_container']['box_bg_color'] = array(
    '#type' => 'color',
    '#title' => t('Box Custom Background Color'),
    '#title_display' => 'invisible',
    '#description' => t('Enter background color'),
    '#element_validate' => array('_fanciblock_validate_color_value'),
    '#default_value' => isset($fanciblock['box_bg_color']) ? $fanciblock['box_bg_color'] : '#efefef',
  );
/*----------- end Box background -----------*/

/*----------- box border -----------*/
  $form['appearance']['box_border_status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Box Border'),
    '#default_value' => isset($fanciblock['box_border_status']) ? $fanciblock['box_border_status'] : false,
  );
  $form['appearance']['box_border_container'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Box Border'), 
    '#attributes' => array('class' => array('container-inline')), 
    '#states' => array(
      'visible' => array(
        ':input[name="block_settings[appearance][box_border_status]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['appearance']['box_border_container']['box_border_width'] = array(
    '#type' => 'select',
    '#title' => t('Box Border Width'),
    '#title_display' => 'invisible',
    '#options' => array('1' => 1, 2, 3, 4, 5, 6, 7, 8),
    '#default_value' => isset($fanciblock['box_border_width']) ? $fanciblock['box_border_width'] : 1,
  );
  $form['appearance']['box_border_container']['box_border_style'] = array(
    '#type' => 'select',
    '#title' => t('Box Border Style'),
    '#title_display' => 'invisible',
    '#options' => array(
      "solid"	=> t('Solid'),
      "dashed"	=> t('Dashed'),
      "dotted"	=> t('Dotted'),
      "double"	=> t('Double'),
    ),
    '#default_value' => isset($fanciblock['box_border_style']) ? $fanciblock['box_border_style'] : 1,
  );
  $form['appearance']['box_border_container']['box_border_color'] = array(
    '#type' => 'color',
    '#title' => t('Box Custom Border Color'),
    '#title_display' => 'invisible',
    '#description' => t('Enter background color'),
    '#element_validate' => array('_fanciblock_validate_color_value'),
    '#default_value' => isset($fanciblock['box_border_color']) ? $fanciblock['box_border_color'] : '#efefef',
  );
/*----------- end box border -----------*/

    // Grid Type
  $form['grid']['grid'] = array(
    '#type' => 'select',
    '#title' => t('Grid Size'),
    '#options' => array('1' => 1, 2, 3, 4, 5, 6, 7, 8),
    '#default_value' => $grid_count,
  );

  // Set up defaults.
  $config = config('fanciblock.settings');
  $default_fid = $config->get('default_fid');
  $default_fanciblock_uri = $config->get('default_fanciblock_uri');
  $default_box = array(
    'fanciblock_name' => $fanciblock['fanciblock_name'], 
    'boxid' => '', 
    'box_title' => '', 
    'weight' => 0, 
    'box_content' => array('value' => 'Hello!'),
    'icon_link' => '',
  );
  if ($delta == 'fanciblock') {
    $default_box += array(
      'fanciblock_fid' => $default_fid, 
      'fanciblock_uri' => $default_fanciblock_uri, 
    );
  }
  elseif($delta == 'fanciicon') {
    $default_box += array(
      'fontawesome_icon' => 'fa-tablet',
      'fontawesome_icon_color' => '#333333',
    );
  }
  form_load_include($form_state, 'inc', 'fanciblock', 'fanciblock.box.admin');
  //$boxes = _fanciblock_add_default_boxes($grid_count, $default_box, $delta);

  $boxes = !empty($fanciblock['boxes']) ? $fanciblock['boxes'] : _fanciblock_add_default_boxes(1, $default_box, $delta);
  dvm($boxes, 'boxes');


  $form['boxes'] = array(
    '#type' => 'value',
    '#value' => $boxes,
  );
  $form['box_edit'] = array(
    '#type' => 'container',
    // todo use parents instead?
    '#tree' => TRUE,
    '#title' => t('Boxes'),
    '#attributes' => array('class' => array('container-inline')),
    '#prefix' => '<div id="fanciblock-box-wrapper">',
    '#suffix' => '</div>',
  );

  foreach ($boxes as $key => $box) {
    $form['box_edit']['grid_' . $key] = array(
      '#type' => 'submit',
      '#value' => 'markup' . $key,
      '#name' => $key,
      '#submit' => array(
        'fanciblock_box_edit_box_open',
      ),
      // '#validate' => array(
        // 'layout_block_configure_form_validate',
      // ),
      '#ajax' => array(
        'callback' => 'fanciblock_ajax_update',
      ),
    );
  }

  // $form['row_settings'] = array(
    // '#type' => 'container',
    // '#id' => 'row_settings',
    // '#parents' => array('row_settings'),
  // );

  // $form['row_settings'] = array(
    // 'row' => backdrop_get_form('fanciblock_box_edit_box', $boxes[0]['boxid']),
  // );

  return $form;
}

