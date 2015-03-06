<?php
/**
 * @file
 * Administrative page callbacks for fanciblock module.
 */

/**
 * Menu callback; Build the form that handles the adding/editing of a fanciblock.
 */
function fanciblock_edit_box($type, $delta, $fanciblock = array()) {
  drupal_add_css(drupal_get_path('module', 'fanciblock') . '/css/fanciblock_admin.css');
  
  //dprint_r($fanciblock);
  // $settings = config_get('fanciblock.settings', $fanciblock['fanciblock_name']);
  $grid_count = isset($fanciblock['grid']) ? $fanciblock['grid'] : 3;

  $fanciblock += array(
    'fanciblock_name' => '',
    'old_name' => !empty($fanciblock['fanciblock_name']) ? $fanciblock['fanciblock_name'] : '',
    'title' => '',
    'optionset' => '',
  );
  $form['fanciblock_type'] = array(
    '#type' => 'hidden',
    '#value' => $delta,
  );

  $form['fanciblock_name'] = array(
    '#type' => 'textfield',
    '#title' => t('fanciblock name'),
    '#default_value' => $fanciblock['fanciblock_name'],
    '#maxlength' => 27,
    '#description' => t('A unique name to construct the URL for the fanciblock. It must only contain lowercase letters, numbers and hyphens.'),
  );

  // Grid Type
  $form['grid']['grid'] = array(
    '#type' => 'select',
    '#title' => t('Grid'),
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
    '#description' => t("Select icon position."),
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

/*----------- icon bg color -----------*/
  $form['appearance']['icon-bg-color'] = array(
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
  $form['appearance']['icon-border-container'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Icon Border'), 
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['appearance']['icon-border-container']['icon-border'] = array(
    '#type' => 'checkbox',
    '#title' => t('Icon Border'),
    '#title_display' => 'invisible',
    '#default_value' => isset($fanciblock['icon-border']) ? $fanciblock['icon-border'] : false,
  );
  $form['appearance']['icon-border-container']['icon-border-width'] = array(
    '#type' => 'select',
    '#title' => t('Icon Border Width'),
    '#title_display' => 'invisible',
    '#options' => array('1' => 1, 2, 3, 4, 5, 6, 7, 8),
    '#default_value' => isset($fanciblock['icon-border-width']) ? $fanciblock['icon-border-width'] : 1,
  );
  $form['appearance']['icon-border-container']['icon-border-style'] = array(
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
  $form['appearance']['icon-border-container']['icon-border-color'] = array(
    '#type' => 'textfield',
    '#title' => t('Icon Custom Border Color'),
    '#title_display' => 'invisible',
    '#size' => 10,
    '#maxlenght' => 4,
    '#description' => t('Enter background color'),
    '#element_validate' => array('_fanciblock_validate_color_value'),
    '#default_value' => isset($fanciblock['icon-border-color']) ? $fanciblock['icon-border-color'] : '#efefef',
  );
/*----------- end icon border -----------*/

/*----------- Box padding -----------*/
  $form['appearance']['box-padding-container'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Box padding'),
    '#attributes' => array('class' => array('container-inline')), 
  );
  $form['appearance']['box-padding-container']['box-padding'] = array(
    '#type' => 'textfield',
    '#title' => t('Box Padding'),
    '#title_display' => 'invisible',
    '#size' => 10,
    '#maxlenght' => 2,
    '#description' => t('Enter box padding'),
    '#element_validate' => array('_fanciblock_validate_color_value'),
    '#default_value' => isset($fanciblock['box-padding']) ? $fanciblock['box-padding'] : '20',
  );
/*----------- end Box padding -----------*/


/*----------- Box background -----------*/
  $form['appearance']['box-bg-container'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Box Background Color'),
    '#attributes' => array('class' => array('container-inline')), 
  );
  $form['appearance']['box-bg-container']['box-bg-color'] = array(
    '#type' => 'textfield',
    '#title' => t('Box Custom Background Color'),
    '#title_display' => 'invisible',
    '#size' => 10,
    '#maxlenght' => 4,
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
  $form['appearance']['box-border-container']['box-border'] = array(
    '#type' => 'checkbox',
    '#title' => t('Box Border'),
    '#title_display' => 'invisible',
    '#default_value' => isset($fanciblock['box-border']) ? $fanciblock['box-border'] : false,
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
    '#maxlenght' => 4,
    '#description' => t('Enter background color'),
    '#element_validate' => array('_fanciblock_validate_color_value'),
    '#default_value' => isset($fanciblock['box-border-color']) ? $fanciblock['box-border-color'] : '#efefef',
  );
/*----------- end box border -----------*/

  return $form;
}

/**
 * Returns whether a fanciblock name already exists.
 *
 * @see fanciblock_edit_box()
 * @see form_validate_machine_name()
 */
function fanciblock_edit_box_name_exists($value) {
  // 'fanciblock-' is added to the fanciblock name to avoid name-space conflicts.
  $value = 'fanciblock-' . $value;
  $fanciblock_exists = db_query_range('SELECT 1 FROM {fanciblock} WHERE fanciblock_name = :fanciblock', 0, 1, array(':fanciblock' => $value))->fetchField();

  return $fanciblock_exists;
}

/**
 * Submit function for the 'Delete' button on the fanciblock editing form.
 */
function fanciblock_delete_box_submit_call($form, &$form_state) {
  $form_state['redirect'] = 'fanciblocks/manage/' . $form_state['values']['fanciblock_name'] . '/delete';
}

/**
 * Menu callback; check access and get a confirm form for deletion of a fanciblock.
 */
function fanciblock_delete_box($form, &$form_state, $fanciblock) {
  $form['#fanciblock'] = $fanciblock;
  $caption = '';
  $caption .= '<p>' . t('This action cannot be undone.') . '</p>';
  return confirm_form($form, t('Are you sure you want to delete the fanciblock %title?', array('%title' => $fanciblock['title'])), 
  'fanciblocks/', $caption, t('Delete'));
}

/**
 * Delete a fanciblock.
 */
function fanciblock_delete_box_submit($form, &$form_state) {
  $fanciblock = $form['#fanciblock'];
  $form_state['redirect'] = 'fanciblocks';

  // Delete the fanciblock and all its boxes.
  fanciblock_delete($fanciblock);

  $t_args = array('%title' => $fanciblock['title']);
  drupal_set_message(t('The fanciblock %title has been deleted.', $t_args));
  watchdog('fanciblock', 'Deleted fanciblock %title and all its boxes.', $t_args, WATCHDOG_NOTICE);
}

/**
 * Validate a form element that should have an integer value.
 */
function _fanciblock_validate_positive_integer($element, &$form_state) {
  $value = $element['#value'];
  if (!is_numeric($value) || intval($value) != $value || $value < 0) {
    form_error($element, t('%name must be a positive integer.', array('%name' => $element['#title'])));
  }
}

/**
 * Validate a form element that should have an valid color value.
 */
function _fanciblock_validate_color_value($element, &$form_state) {
  $value = $element['#value'];
  // Only accept hexadecimal CSS color strings.
  if (!preg_match('/^#([a-f0-9]{3}){1,2}$/iD', $value) && $value != 'transparent') {
    form_error($element, t('%name must be a valid hexadecimal CSS color value.', array('%name' => $element['#title'])));
  }
}