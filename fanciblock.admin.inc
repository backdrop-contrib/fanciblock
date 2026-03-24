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
function fanciblock_edit_box(&$form, &$form_state, $type, $delta, $fanciblock = array()) {
  backdrop_add_css(backdrop_get_path('module', 'fanciblock') . '/css/fanciblock.admin.css');

  $fanciblock += fanciblock_default_settings();

  $fanciblock['fanciblock_type'] = $delta;
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
  $fanciblock['fanciblock_name'] = isset($form_state['block']->uuid) ? $form_state['block']->uuid : '';
  $form['fanciblock_name'] = array(
    '#value' => $fanciblock['fanciblock_name'],
    '#type' => 'value',
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
        "small" => t('Medium (75x75)'),
        "tiny"  => t('Small (40x40)'),
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
        ':input[name="appearance[icon_style]"]' => array('value' => 'none'),
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
        ':input[name="appearance[icon_bg_color_wrapper][icon_bg_color_status]"]' => array('checked' => TRUE),
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
        ':input[name="appearance[icon_style]"]' => array('value' => 'none'),
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
        ':input[name="appearance[icon_border_wrapper][icon_border_status]"]' => array('checked' => TRUE),
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
        ':input[name="appearance[box_bg_color_status]"]' => array('checked' => TRUE),
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
        ':input[name="appearance[box_border_status]"]' => array('checked' => TRUE),
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

  $form_state['fanciblock_name'] = $fanciblock['fanciblock_name'];
  $form_state['fanciblock'] = $fanciblock;
  $form_state['delta'] = $delta;

  $fanciblock['boxes'] = !empty($fanciblock['boxes']) ? $fanciblock['boxes'] : _fanciblock_add_default_boxes(1, $fanciblock, $delta);
  $form_state['boxes'] = !empty($form_state['boxes']) ? $form_state['boxes'] : $fanciblock['boxes'];


  $not_saved = empty($form_state['block']->uuid);
  $fanciblock['disabled'] = $not_saved;

  $form['not_saved_message'] = array(
    '#type' => 'help',
    '#markup' => t('Save this block to enable editing boxes'),
    '#access' => $not_saved,
  );
  $form['box_edit'] = array(
    '#type' => 'container',
    // todo use parents instead?
    '#tree' => TRUE,
    '#title' => t('Boxes'),
    '#attributes' => array('class' => array('fanciblock-box-wrapper')),
    '#prefix' => '<div id="fanciblock-box-wrapper">',
    '#suffix' => '</div>',
  );

  $form['#attached']['js'][] = backdrop_get_path('module', 'layout') . '/js/layout.admin.js';

  foreach ($form_state['boxes'] as $key => $box) {
    $form['box_edit']['grid_' . $key] = array(
      '#type' => 'container',
      '#theme' => array('fanciblock_box_preview'),
      '#attributes' => array('class' => array('fanciblock-box-preview-container')),
      '#fanciblock' => $fanciblock,
      '#box' => $box,
    );
    $form['box_edit']['grid_' . $key]['button'] = array(
      '#type' => 'submit',
      '#value' => t('Edit'),
      '#name' => $key,
      '#access' => !$not_saved,
      '#attributes' => array('class' => array('layout-link-button')),
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
    $form['box_edit']['grid_' . $key]['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#name' => $key,
      '#access' => !$not_saved,
      '#attributes' => array('class' => array('layout-link-button')),
      '#limit_validation_errors' => array(),
      '#submit' => array(
        'fanciblock_delete_box',
      ),
      '#ajax' => array(
        'callback' => 'fanciblock_box_add_delete_ajax',
        'wrapper' => 'fanciblock-box-wrapper',
      ),
    );
  }

  $form['add_additional_box'] = array(
    '#type' => 'submit',
    '#value' => t('Add box'),
    '#access' => !$not_saved,
    '#attributes' => array(
      'class' => array('layout-link-button', 'layout-add-box')
    ),
    '#validate' => array(),
    '#submit' => array(
      'fanciblock_add_additional_box',
    ),
    '#ajax' => array(
      'callback' => 'fanciblock_box_add_delete_ajax',
      'wrapper' => 'fanciblock-box-wrapper',
    ),
  );

  return $form;
}
