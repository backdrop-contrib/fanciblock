<?php

/**
 * Returns a small icon for list views.
 */
function _fanciicon_fontawesome_icon_build($box) {
  if($box['fanciblock_type'] == 'fanciicon') {
    return theme('fanciblock_fontawesome_icon_build', array('class' => $box['fontawesome_icon']));
  }
  elseif($box['fanciblock_type'] == 'fanciicon') {
    return theme('image_style', array('uri' => $box['fanciblock_uri'], 'style_name' => 'tiny_40x40'));
  }
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

/**
 * Menu callback; Build the fanciblock box editing form.
 */
function fanciblock_box_add_box($fanciblock) {
  return backdrop_get_form('fanciblock_box_edit_box', $fanciblock);
}

/**
 * Menu callback; Build the fanciblock box editing form.
 */
function fanciblock_box_edit_box($form, &$form_state, $fanciblock, $boxid = NULL, $type = 'edit') {
  backdrop_add_css(backdrop_get_path('module', 'fanciblock') . '/css/fanciblock.admin.css');
  $fanciblock_boxes = $fanciblock['boxes'];
  $box = $fanciblock_boxes[$boxid];
  dpm($box);

  $form['boxid'] = array(
    '#type' => 'hidden',
    '#value' => $boxid,
  );
  $form['fanciblock_name'] = array(
    '#type' => 'hidden',
    '#value' => $fanciblock['fanciblock_name'],
  );
  $form['fanciblock_type'] = array(
    '#type' => 'hidden',
    '#value' => $fanciblock['fanciblock_type'],
  );
  $form['fanciblock'] = array(
    '#type' => 'value',
    '#value' => $fanciblock,
  );
  $form['box_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Box title'),
    '#default_value' => isset($box['box_title']) ? $box['box_title'] : '',
    '#description' => t('The human-readable name of this box'),
    '#required' => TRUE,
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#delta' => 50,
    '#default_value' => isset($box['weight']) ? $box['weight'] : '',
    '#description' => t('Optional. Boxes are displayed in ascending order by weight.'),
  );
  $form['box_content'] = array(
    '#type' => 'text_format',
    '#format' => 'filtered_html',
    '#title' => t('Box Content'),
    '#default_value' => isset($box['box_content']['value']) ? $box['box_content']['value'] : '',
  );
  if($fanciblock['fanciblock_type'] == 'fanciblock') {
    $form['fanciblock_uri'] = array(
      '#title' => t('Image'),
      '#type' => 'managed_file',
      '#description' => t('The uploaded image will be displayed on this page using the image style choosen below.'),
      '#default_value' => isset($box['fanciblock_fid']) ? $box['fanciblock_fid'] : NULL,
      '#upload_location' => 'public://fanciblock_images/',
    );
  }
  elseif($fanciblock['fanciblock_type'] == 'fanciicon') {
    $core_icons = scandir('core/misc/icons');
    foreach ($core_icons as $icon) {
      if (substr($icon, -4, 4) == '.svg') {
        $icon_name = substr($icon, 0, -4);
        if (substr($icon_name, -4, 4) == 'fill') {
          continue;
        }
        $icons[$icon_name] = fanciblock_icon_display_name($icon_name);
      }
    }

    $form['fontawesome_icon'] = array(
      '#type' => 'select',
      '#title' => t('Font Awesome icon'),
      '#options' => $icons,
      '#default_value' => isset($box['fontawesome_icon']) ? $box['fontawesome_icon'] : '',
      '#description' => t('Please select font awesome icon. For more info see: <a href="http://fortawesome.github.io/Font-Awesome/cheatsheet/" target="_blank">Font Awesome Cheatsheet</a>.'),
    );
    $form['fontawesome_icon_color'] = array(
      '#type' => 'color',
      '#title' => t('Font Awesome icon color'),
      '#default_value' => isset($box['fontawesome_icon_color']) ? $box['fontawesome_icon_color'] : '',
      '#description' => t('Please select icon color.'),
      '#element_validate' => array('_fanciblock_validate_color_value'),
    );
  }
  $form['icon_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Icon Link'),
    '#default_value' => isset($box['icon_link']) ? $box['icon_link'] : '',
    '#description' => t('Optional path to which the icon or image will be linked.'),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array(
      'fanciblock_box_edit_box_submit',
    ),
    '#ajax' => array(
      'callback' => 'fanciblock_ajax_update',
    ),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#limit_validation_errors' => array(),
    //'#attributes' => array('class' => array('layout-link-button')),
    '#submit' => array(
      'fanciblock_box_edit_box_cancel_submit',
    ),
    '#ajax' => array(
      'callback' => 'fanciblock_ajax_update',
    ),
  );
  return $form;
}
