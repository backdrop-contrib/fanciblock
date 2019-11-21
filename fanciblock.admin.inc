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
  drupal_add_css(drupal_get_path('module', 'fanciblock') . '/css/fanciblock_admin.css');

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

    // Grid Type
  $form['grid']['grid'] = array(
    '#type' => 'select',
    '#title' => t('Grid Size'),
    '#options' => array('1' => 1, 2, 3, 4, 5, 6, 7, 8),
    '#default_value' => $grid_count,
  );

  // Appearance fanciblock
  $form['appearance'] = array(
    '#type' => 'fieldset',
    '#title' => t('appearance'),
  );
  $form['appearance']['icon-position'] = array(
    '#type' => 'select',
    '#title' => t('Icon Position'),
    '#options' => array(
      "top"  => t('Top'),
      "left" => t('Left'),
      "right"  => t('Right'),
      "boxcut" => t('Boxcut'),
    ),
    '#default_value' => isset($fanciblock['icon-position']) ? $fanciblock['icon-position'] : 'top',
  );
  $form['appearance']['icon-style'] = array(
    '#type' => 'select',
    '#title' => t('Icon Style'),
    '#options' => array(
      "none" => t('None'),
      "circle" => t('Circle'),
      "square"  => t('Square'),
    ),
    '#default_value' => isset($fanciblock['icon-style']) ? $fanciblock['icon-style'] : 'none',
  );
  
  if($delta == 'fanciblock') {
  /*----------- icon size -----------*/
    $form['appearance']['icon-size'] = array(
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
      '#default_value' => isset($fanciblock['icon-size']) ? $fanciblock['icon-size'] : 'medium',
    );
  /*----------- end icon size -----------*/
  }
  elseif($delta == 'fanciicon') {
  /*----------- icon size -----------*/
    $form['appearance']['icon-size'] = array(
      '#type' => 'select',
      '#title' => t('Icon Size'),
      '#options' => array(
        "xlarge"  => t('Extra Large (150x150)'),
        "large"  => t('Large (110x110)'),
        "medium" => t('Medium (70x70)'),
        "small"  => t('Small (45x45)'),
        "tiny"  => t('Tiny (25x25)'),
      ),
      '#default_value' => isset($fanciblock['icon-size']) ? $fanciblock['icon-size'] : 'medium',
    );
  /*----------- end icon size -----------*/
  }
/*----------- Box padding -----------*/
  $form['appearance']['box-padding'] = array(
    '#type' => 'textfield',
    '#title' => t('Box Padding'),
    '#size' => 10,
    '#maxlength' => 2,
    '#default_value' => isset($fanciblock['box-padding']) ? $fanciblock['box-padding'] : '20',
  );
/*----------- end Box padding -----------*/

/*----------- icon bg color -----------*/
  $form['appearance']['icon-bg-color-wrapper'] = array(
    '#type' => 'container',
    '#title' => t('Icon Background Color'),
    '#states' => array(
      'invisible' => array(
        ':input[name="block_settings[appearance][icon-style]"]' => array('value' => 'none'),
      ),
    ),
  );
  $form['appearance']['icon-bg-color-wrapper']['icon-bg-color-status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Icon Background Color'),
    '#default_value' => isset($fanciblock['icon-bg-color-status']) ? $fanciblock['icon-bg-color-status'] : false,
  );
  $form['appearance']['icon-bg-color-wrapper']['icon-bg-color-container'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Icon Background Color'), 
    '#title_display' => 'invisible',
    '#attributes' => array('class' => array('container-inline')),
    '#states' => array(
      'visible' => array(
        ':input[name="block_settings[appearance][icon-bg-color-wrapper][icon-bg-color-status]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['appearance']['icon-bg-color-wrapper']['icon-bg-color-container']['icon-bg-color'] = array(
    '#type' => 'textfield',
    '#title' => t('Icon Background Color'),
    '#size' => 10,
    '#maxlength' => 8,
    '#description' => t('Enter background color'),
    '#element_validate' => array('_fanciblock_validate_color_value'),
    '#default_value' => isset($fanciblock['icon-bg-color']) ? $fanciblock['icon-bg-color'] : '#333333',
  );
/*----------- end icon bg color -----------*/

/*----------- icon border -----------*/
  $form['appearance']['icon-border-wrapper'] = array(
    '#type' => 'container',
    '#title' => t('Icon Background Color'),
    '#states' => array(
      'invisible' => array(
        ':input[name="block_settings[appearance][icon-style]"]' => array('value' => 'none'),
      ),
    ),
  );
  $form['appearance']['icon-border-wrapper']['icon-border-status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Icon Border'),
    '#default_value' => isset($fanciblock['icon-border']) ? $fanciblock['icon-border'] : false,
  );
  $form['appearance']['icon-border-wrapper']['icon-border-container'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Icon Border Settings'), 
    '#title_display' => 'invisible',
    '#attributes' => array('class' => array('container-inline')),
    '#states' => array(
      'visible' => array(
        ':input[name="block_settings[appearance][icon-border-wrapper][icon-border-status]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['appearance']['icon-border-wrapper']['icon-border-container']['icon-border-width'] = array(
    '#type' => 'select',
    '#title' => t('Icon Border Width'),
    '#title_display' => 'invisible',
    '#options' => array('1' => 1, 2, 3, 4, 5, 6, 7, 8),
    '#default_value' => isset($fanciblock['icon-border-width']) ? $fanciblock['icon-border-width'] : 1,
  );
  $form['appearance']['icon-border-wrapper']['icon-border-container']['icon-border-style'] = array(
    '#type' => 'select',
    '#title' => t('Icon Border Style'),
    '#title_display' => 'invisible',
    '#options' => array(
      "solid"	=> t('Solid'),
      "dashed"	=> t('Dashed'),
      "dotted"	=> t('Dotted'),
      "double"	=> t('Double'),
    ),
    '#default_value' => isset($fanciblock['icon-border-style']) ? $fanciblock['icon-border-style'] : 1,
  );
  $form['appearance']['icon-border-wrapper']['icon-border-container']['icon-border-color'] = array(
    '#type' => 'textfield',
    '#title' => t('Icon Custom Border Color'),
    '#title_display' => 'invisible',
    '#size' => 10,
    '#maxlength' => 8,
    '#description' => t('Enter background color'),
    '#element_validate' => array('_fanciblock_validate_color_value'),
    '#default_value' => isset($fanciblock['icon-border-color']) ? $fanciblock['icon-border-color'] : '#efefef',
  );
/*----------- end icon border -----------*/


/*----------- Box background -----------*/
  $form['appearance']['box-bg-color-status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Box Background Color'),
    '#default_value' => isset($fanciblock['box-bg-color-status']) ? $fanciblock['box-bg-color-status'] : false,
  );
  $form['appearance']['box-bg-color-container'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Box Background Color'),
    '#title_display' => 'invisible',
    '#attributes' => array('class' => array('container-inline')),
    '#states' => array(
      'visible' => array(
        ':input[name="block_settings[appearance][box-bg-color-status]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['appearance']['box-bg-color-container']['box-bg-color'] = array(
    '#type' => 'textfield',
    '#title' => t('Box Custom Background Color'),
    '#title_display' => 'invisible',
    '#size' => 10,
    '#maxlength' => 8,
    '#description' => t('Enter background color'),
    '#element_validate' => array('_fanciblock_validate_color_value'),
    '#default_value' => isset($fanciblock['box-bg-color']) ? $fanciblock['box-bg-color'] : '#efefef',
  );
/*----------- end Box background -----------*/

/*----------- box border -----------*/
  $form['appearance']['box-border-status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Box Border'),
    '#default_value' => isset($fanciblock['box-border-status']) ? $fanciblock['box-border-status'] : false,
  );
  $form['appearance']['box-border-container'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Box Border'), 
    '#attributes' => array('class' => array('container-inline')), 
    '#states' => array(
      'visible' => array(
        ':input[name="block_settings[appearance][box-border-status]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['appearance']['box-border-container']['box-border-width'] = array(
    '#type' => 'select',
    '#title' => t('Box Border Width'),
    '#title_display' => 'invisible',
    '#options' => array('1' => 1, 2, 3, 4, 5, 6, 7, 8),
    '#default_value' => isset($fanciblock['box-border-width']) ? $fanciblock['box-border-width'] : 1,
  );
  $form['appearance']['box-border-container']['box-border-style'] = array(
    '#type' => 'select',
    '#title' => t('Box Border Style'),
    '#title_display' => 'invisible',
    '#options' => array(
      "solid"	=> t('Solid'),
      "dashed"	=> t('Dashed'),
      "dotted"	=> t('Dotted'),
      "double"	=> t('Double'),
    ),
    '#default_value' => isset($fanciblock['box-border-style']) ? $fanciblock['box-border-style'] : 1,
  );
  $form['appearance']['box-border-container']['box-border-color'] = array(
    '#type' => 'textfield',
    '#title' => t('Box Custom Border Color'),
    '#title_display' => 'invisible',
    '#size' => 10,
    '#maxlength' => 8,
    '#description' => t('Enter background color'),
    '#element_validate' => array('_fanciblock_validate_color_value'),
    '#default_value' => isset($fanciblock['box-border-color']) ? $fanciblock['box-border-color'] : '#efefef',
  );
/*----------- end box border -----------*/

  return $form;
}
