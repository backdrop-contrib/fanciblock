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
  
  if ($result) {
    backdrop_add_library('system', 'backdrop.ajax');
    module_load_include('inc', 'layout', 'layout.admin');
    $usage = layout_get_block_usage('fanciblock');

    $header = array(t('Fanciblock name'), t('Usage'), t('Preview'), array('data' => t('Operations')));
    $rows = array();
    foreach ($result as $key => $fanciblock) {
      if (is_array($fanciblock)) {
        $row = array();

        $preview = array(
          '#type' => 'link',
          '#title' => t('Preview'),
          '#href' => 'fanciblock/preview/' . $key,
          '#attributes' => array(
            'class' => array('use-ajax'),
            'data-dialog' => 'true',
            'data-dialog-options' => json_encode(array('width' => 700, 'dialogClass' => 'project-dialog')),
          ),
        );

        if (isset($usage[$key])) {
          $layout_titles = array();
          foreach ($usage[$key] as $layout_name => $layout) {
            foreach ($layout as $region) {
              foreach ($region as $block) {
                $layout_titles[$layout_name] = $block->layout_title;
              }
            }
          }
          $usages = implode(', ', $layout_titles);
        }
        else {
          $usages = t('Not in use');
        }

        $links = array();
        $links['edit_boxes'] = array(
          'title' => t('Manage boxes'),
          'href' => 'admin/structure/fanciblocks/' . $key,
        );
        $links['edit'] = array(
          'title' => t('Edit block'),
          'href' => 'admin/structure/fanciblocks/' . $key . '/edit',
        );
        $links['delete'] = array(
          'title' => t('Delete block'),
          'href' => 'admin/structure/fanciblocks/' . $key . '/delete',
        );
        $operations = array(
          '#type' => 'dropbutton',
          '#links' => $links,
        );

        $row[] = check_plain($fanciblock['settings']['fanciblock_title']);
        $row[] = $usages;
        $row[] = backdrop_render($preview);
        $row[] = backdrop_render($operations);
        $rows[] = $row;
      }
    }
  }
  else {
    $header = array(t('Fanciblock name'), t('Usage'), t('Preview'), array('data' => t('Operations')));
    $rows = array();
    $rows[] = array(array('data' => t('There are no fanciblocks yet')));
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
function fanciblock_box_add_block($type) {
  return backdrop_get_form('fanciblock_block_edit_form', array(), $type);
}

/**
 * Menu callback; Build the form that handles the adding/editing of a fanciblock.
 */
function fanciblock_block_edit_form($form, &$form_state, $fanciblock = array(), $type = NULL) {
  form_load_include($form_state, 'inc', 'fanciblock', 'fanciblock.admin');
  backdrop_add_css(backdrop_get_path('module', 'fanciblock') . '/css/fanciblock_admin.css');

  $grid_count = isset($fanciblock['boxes']) ? count($fanciblock['boxes']) : 3;


  $fanciblock = !empty($fanciblock['settings']) ? $fanciblock['settings'] : array();
  $type = $type ? $type : $fanciblock['fanciblock_type'];

  $form = array();
  $form['fanciblock_type'] = array(
    '#type' => 'hidden',
    '#value' => $type,
  );
  $form['fanciblock'] = array(
    '#type' => 'hidden',
    '#value' => $fanciblock,
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
      'source' => array('fanciblock_title'),
    ),
  );

    // Grid Type
  $form['grid'] = array(
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

  if ($type == 'fanciblock') {
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
  elseif ($type == 'fanciicon') {
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
  $form['appearance']['box-padding'] = array(
    '#type' => 'textfield',
    '#title' => t('Box Padding'),
    '#size' => 10,
    '#maxlength' => 2,
    '#default_value' => isset($fanciblock['box-padding']) ? $fanciblock['box-padding'] : '20',
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

/*----------- icon bg color -----------*/
  $form['appearance']['icon-bg-color-wrapper'] = array(
    '#type' => 'container',
    '#title' => t('Icon Background Color'),
    '#states' => array(
      'invisible' => array(
        ':input[name="icon-style"]' => array('value' => 'none'),
      ),
    ),
  );
  $form['appearance']['icon-bg-color-wrapper']['icon-bg-color-container'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Icon Background Color'), 
    '#title_display' => 'invisible',
    '#attributes' => array('class' => array('container-inline')),
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
        ':input[name="icon-style"]' => array('value' => 'none'),
      ),
    ),
  );
  $form['appearance']['icon-border-wrapper']['icon-border-container'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Icon Border Settings'), 
    '#title_display' => 'invisible',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['appearance']['icon-border-wrapper']['icon-border-container']['icon-border-width'] = array(
    '#type' => 'select',
    '#title' => t('Icon Border Width'),
    '#title_display' => 'invisible',
    '#options' => array(0 => t('No icon border'), 1, 2, 3, 4, 5, 6, 7, 8),
    '#default_value' => isset($fanciblock['icon-border-width']) ? $fanciblock['icon-border-width'] : 0,
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
    '#states' => array(
      'invisible' => array(
        ':input[name="icon-border-width"]' => array('value' => 0),
      ),
    ),
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
    '#states' => array(
      'invisible' => array(
        ':input[name="icon-border-width"]' => array('value' => 0),
      ),
    ),
  );
/*----------- end icon border -----------*/


/*----------- Box background -----------*/
  $form['appearance']['box-bg-color-container'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Box Background Color'),
    '#title_display' => 'invisible',
    '#attributes' => array('class' => array('container-inline')),
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
  $form['appearance']['box-border-container'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Box Border'), 
    '#attributes' => array('class' => array('container-inline')), 
  );
  $form['appearance']['box-border-container']['box-border-width'] = array(
    '#type' => 'select',
    '#title' => t('Box Border Width'),
    '#title_display' => 'invisible',
    '#options' => array(0 => t('No icon border'), 1, 2, 3, 4, 5, 6, 7, 8),
    '#default_value' => isset($fanciblock['box-border-width']) ? $fanciblock['box-border-width'] : 0,
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
    '#states' => array(
      'invisible' => array(
        ':input[name="box-border-width"]' => array('value' => 0),
      ),
    ),
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
    '#states' => array(
      'invisible' => array(
        ':input[name="box-border-width"]' => array('value' => 0),
      ),
    ),
  );
/*----------- end box border -----------*/

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('fanciblock_block_edit_form_submit'),
  );
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#access' => !empty($fanciblock),
    '#submit' => array('fanciblock_block_delete_redirect'),
  );

  return $form;
}

/**
 *  Saves a block (fanciblock).
 *  
 *  @param $type The type of block being saved.
 */
function fanciblock_block_edit_form_submit($form, &$form_state) {
  $type = $form_state['values']['fanciblock_type'];
  form_state_values_clean($form_state);
  $values = $form_state['values'];

  if ($type == 'fanciblock' || $type == 'fanciicon') {
    $config = config('fanciblock.settings');
    
    // Set up defaults.
    $default_fid = $config->get('default_fid');
    $default_fanciblock_uri = $config->get('default_fanciblock_uri');
    $default_box = array(
      'fanciblock_name' => $values['fanciblock_name'], 
      'boxid' => '', 
      'box_title' => $values['fanciblock_title'], 
      'weight' => 0, 
      'box_content' => array('value' => 'Hello!'),
      'icon_link' => '',
    );
    if ($type == 'fanciblock') {
      $default_box += array(
        'fanciblock_fid' => $default_fid, 
        'fanciblock_uri' => $default_fanciblock_uri, 
      );
    }
    elseif ($type == 'fanciicon') {
      $default_box += array(
        'fontawesome_icon' => 'fa-tablet',
        'fontawesome_icon_color' => '#333333',
      );
    }
    
    // Check and see if this block's settings already exist. Theyre saved in
    // config, keyed by the block's unique name (fanciblock_name).
    $settings = $config->get($values['fanciblock_name']);
    // If we dont find the settings, this is a new block being built, so
    // build and save a default box for each grid.
    if (!$settings) {
      $boxes = _fanciblock_add_default_boxes($values['grid'], $default_box, $type);
      $settings['boxes'] = $boxes;
    }
    // If we are adding boxes, grid number will be more than the number of 
    // settings.
    elseif ($values['grid'] > count($settings['boxes'])) {
      $adding = $values['grid'] - count($settings['boxes']);
      $boxes = _fanciblock_add_default_boxes($adding, $default_box, $type);
      $settings['boxes'] += $boxes;
    }
    // Theres no safe way to decrease the number of boxes, so we dont allow that.
    elseif ($values['grid'] < count($settings['boxes'])) {
      form_set_error('', 'Cannot decrease number of boxes, please delete box instead');
    }
    $settings['settings'] = $values;
    fanciblock_box_save($settings);
  }
  $form_state['redirect'] = 'admin/structure/fanciblocks';
}

/**
 * Form submission handler for fanciblock_block_edit_form() to delete a
 * fanciblock.
 */
function fanciblock_block_delete_redirect($form, &$form_state) {
  $fanciblock = $form_state['values']['fanciblock'];
  dpm($fanciblock);
  $form_state['redirect'] = 'admin/structure/fanciblocks/' . $fanciblock['fanciblock_name'] . '/delete';
}

/**
 * Menu callback; Delete a fanciblock.
 *
 * @ingroup forms
 */
function fanciblock_block_delete_confirm($form, &$form_state, $fanciblock) {
  $form_state['fanciblock'] = $fanciblock;

  $question = t('Delete fanciblock @title?', array('@title' => $fanciblock['settings']['fanciblock_title']));

  return confirm_form($form, $question, 'admin/structure/fanciblocks');
}


/**
 * Form callback; Delete a fanciblock.
 *
 * @ingroup forms
 */
function fanciblock_block_delete_confirm_submit(&$form, &$form_state) {
  $fanciblock = $form_state['fanciblock'];
  $config = config('fanciblock.settings');
  $config->clear($fanciblock['settings']['fanciblock_name']);
  $config->save();

  $form_state['redirect'] = 'admin/structure/fanciblocks';
}

