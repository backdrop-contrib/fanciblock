<?php
/**
 * Form for editing an entire fanciblock box order at once.
 */
function fanciblock_box_overview_form($form, &$form_state, $fanciblock) {
  $form['fanciblock_box']['#tree'] = TRUE;
  
  $boxes = $fanciblock['boxes'];
  if (!empty($boxes)) {
    backdrop_sort($boxes, array('weight' => SORT_NUMERIC));
  }
  
  $form['boxes'] = array(
    '#type' => 'value',
    '#value' => $boxes,
  );
    
  $form['fanciblock'] = array(
    '#type' => 'value',
    '#value' => $fanciblock,
  );
    
  foreach ($boxes as $box) {
    $form['fanciblock_box'][$box['boxid']] = array(
      'box_title' => array(
        '#markup' => check_plain($box['box_title']),
      ),
      'box_content' => array(
        '#markup' => filter_xss($box['box_content']['value']),
      ),
      'fontawesome_icon' => array(
        '#markup' => _fanciicon_fontawesome_icon_build($box),
      ),
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $box['weight'],
        '#delta' => 50,
        '#title-display' => 'invisible',
      ),
      'edit' => array('#type' => 'link', '#title' => t('edit'), '#href' => "admin/config/media/fanciblocks/" . $box['fanciblock_name'] . "/". $box['boxid'] ."/edit"),
      'delete' => array('#type' => 'link', '#title' => t('delete'), '#href' => "admin/config/media/fanciblocks/" . $box['fanciblock_name'] . "/". $box['boxid'] ."/delete"),
    );
  }

  if (element_children($form['fanciblock_box'])) {
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );
  }
  else {
    $form['#empty_text'] = t('There are no boxes yet. <a href="@link">Add a box</a>.', 
    array('@link' => url('admin/config/media/fanciblocks/'. $fanciblock['settings']['fanciblock_name'] .'/add-box')));
  }
  return $form;
}

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
  $output = '<i class="fa ';
  $output .= $variables['class'] . ' ';
  $output .= $variables['size'] . ' ';
  $output .= $variables['looks'] . '" ';
  $output .= $variables['style'] . '></i>';

  return $output;
}

/**
 * Submit handler for the box overview form.
 * @see box_overview_form()
 */
function fanciblock_box_overview_form_submit($form, &$form_state) {
  $fanciblock = $form_state['values']['fanciblock'];
  $weights = $form_state['values']['fanciblock_box'];
  $boxes = $form_state['values']['boxes'];

  foreach ($boxes as $key => $box) {
    $boxes[$key]['weight'] = $weights[$key]['weight'];
  }
  $fanciblock['boxes'] = $boxes;

  fanciblock_box_save($fanciblock);
}

/**
 * Returns HTML for the box overview form into a table.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_fanciblock_box_overview_form($variables) {
  backdrop_add_library('fanciblock', 'fontawesome'); // load fontawesome css file

  $form = $variables['form'];

  $rows = array();
  foreach (element_children($form['fanciblock_box']) as $boxid) {
    $form['fanciblock_box'][$boxid]['weight']['#attributes']['class'] = array('fanciblock-box-weight');
    $rows[] = array(
      'data' => array(
        backdrop_render($form['fanciblock_box'][$boxid]['box_title']),
        backdrop_render($form['fanciblock_box'][$boxid]['fontawesome_icon']),
        backdrop_render($form['fanciblock_box'][$boxid]['box_content']),
        backdrop_render($form['fanciblock_box'][$boxid]['weight']),
        backdrop_render($form['fanciblock_box'][$boxid]['edit']),
        backdrop_render($form['fanciblock_box'][$boxid]['delete']),
      ),
      'class' => array('draggable'),
    );
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => $form['#empty_text'], 'colspan' => '5'));
  }

  $header = array(t('Box Title'), t('Icon'), t('Box Content'), t('Weight'), t('Edit'), t('Delete'));
  $table_id = 'fanciblock-overview';

  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => $table_id),
  ));
  $output .= backdrop_render_children($form);
  backdrop_add_tabledrag($table_id, 'order', 'sibling', 'fanciblock-box-weight');
  return $output;
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

  $form['boxid'] = array(
    '#type' => 'hidden',
    '#value' => $boxid,
  );
  $form['fanciblock_name'] = array(
    '#type' => 'hidden',
    '#value' => $fanciblock['settings']['fanciblock_name'],
  );
  $form['fanciblock_type'] = array(
    '#type' => 'hidden',
    '#value' => $fanciblock['settings']['fanciblock_type'],
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
  if($fanciblock['settings']['fanciblock_type'] == 'fanciblock') {
    $form['fanciblock_uri'] = array(
      '#title' => t('Image'),
      '#type' => 'managed_file',
      '#description' => t('The uploaded image will be displayed in this block.'),
      '#default_value' => isset($box['fanciblock_fid']) ? $box['fanciblock_fid'] : NULL,
      '#upload_location' => 'public://fanciblock_images/',
    );
  }
  elseif($fanciblock['settings']['fanciblock_type'] == 'fanciicon') {
    $form['fontawesome_icon'] = array(
      '#type' => 'select',
      '#title' => t('Font Awesome icon'),
      '#options' => fanciblock_box_fontawesome_parser(backdrop_get_path('module', 'fanciblock') . '/fontawesome/css/font-awesome.css'),
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
  );
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#access' => $type == 'edit',
    '#submit' => array('fanciblock_box_delete_box_submit_call'),
  );
  return $form;
}

/**
 * Submit function for the delete button on the box editing form.
 */
function fanciblock_box_delete_box_submit_call($form, &$form_state) {
  $_GET['destination'] = 'admin/config/media/fanciblocks/' . $form_state['values']['fanciblock_name'] . '/' . $form_state['values']['boxid'] . '/delete';
}

/**
 * Process fanciblock box add/edit form submissions.
 */
function fanciblock_box_edit_box_submit($form, &$form_state) {
  // The entire fancibox
  $fanciblock = $form_state['values']['fanciblock'];
  global $user;
  
  // Save common form values
  $box = array(
    'fanciblock_name' => $form_state['values']['fanciblock_name'],
    'box_title' => $form_state['values']['box_title'],
    'weight' => $form_state['values']['weight'],
    'box_content' => $form_state['values']['box_content'],
    'icon_link' => $form_state['values']['icon_link'],
    'boxid' => $form_state['values']['boxid'] ? $form_state['values']['boxid'] : uniqid(),
    'fanciblock_type' => $form_state['values']['fanciblock_type'],
  );
  // Save type-specific values. Start with fanciblock.
  if($form_state['values']['fanciblock_type'] == 'fanciblock') {
    // If the image field is set, save the image.
    if($form_state['values']['fanciblock_uri']) {
      $file = file_load($form_state['values']['fanciblock_uri']);
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      file_usage_add($file, 'user', 'user', $user->uid);
      $box['fanciblock_fid'] = $file->fid;
      $box['fanciblock_uri'] = $file->uri;
    }
    // Otherwise keep the previous values. (We avoid empty boxes.)
    else {
      $box['fanciblock_fid'] = $fanciblock['boxes'][$form_state['values']['boxid']]['fanciblock_fid'];
      $box['fanciblock_uri'] = $fanciblock['boxes'][$form_state['values']['boxid']]['fanciblock_uri'];
    }
  }
  // If its a fanciicon, save the icon value. Its a select so its never empty.
  elseif($form_state['values']['fanciblock_type'] == 'fanciicon') {
    $box['fontawesome_icon'] = $form_state['values']['fontawesome_icon'];
    $box['fontawesome_icon_color'] = $form_state['values']['fontawesome_icon_color'];
  }
  $fanciblock['boxes'][$box['boxid']] = $box;
  fanciblock_box_save($fanciblock);
  $form_state['redirect'] = 'admin/config/media/fanciblocks/' . $box['fanciblock_name'];
  backdrop_set_message(t('Your configuration has been saved.'));
}

/**
 * Menu callback; check access and get a confirm form for deletion of a box.
 */
function fanciblock_box_delete_box($form, &$form_state, $fanciblock, $boxid, $type) {
  $form['#fanciblock_name'] = $fanciblock['settings']['fanciblock_name'];
  $form['#boxid'] = $boxid;
  return confirm_form($form, t('Are you sure you want to delete the box %box?', array('%box' => $fanciblock['boxes'][$form['#boxid']]['box_title'])), 'admin/config/media/fanciblocks/' . $form['#fanciblock_name'] . '/' . $boxid . '/edit');
}

/**
 * Process box delete form submissions.
 */
function fanciblock_box_delete_box_submit($form, &$form_state) {
  $fanciblock = config_get('fanciblock.settings', $form['#fanciblock_name']);
  // Save the name for backdrop_set_message() and watchdog().
  $deleted = $fanciblock['boxes'][$form['#boxid']]['box_title'];
  // Now we have to reduce the grid number by one since we are deleting a grid.
  $grid = $fanciblock['settings']['grid']['grid']--;
  // Do the delete.
  unset($fanciblock['boxes'][$form['#boxid']]);
  fanciblock_box_save($fanciblock);
  $t_args = array('%title' => $deleted);
  backdrop_set_message(t('The fanciblock box %title has been deleted.', $t_args));
  watchdog('fanciblock', 'Deleted fanciblock box %title.', $t_args, WATCHDOG_NOTICE);
  backdrop_goto('admin/config/media/fanciblocks/' . $form['#fanciblock_name']);
}

/**
 * CSS parser from the file of css.
 */
function fanciblock_box_fontawesome_parser($path) {
  $css = backdrop_load_stylesheet($path, TRUE);

  preg_match_all( '/(?ims)([a-z0-9\s\.\:#_\-@,]+)\{([^\}]*)\}/', $css, $arr);
  $result = array();
  foreach ($arr[0] as $i => $x) {
    $selector = trim($arr[1][$i]);
    $rules = explode(';', trim($arr[2][$i]));
    $rules_arr = array();
    foreach ($rules as $strRule) {
      if (!empty($strRule)) {
        $rule = explode(":", $strRule);
        $rules_arr[trim($rule[0])] = trim($rule[1]);
      }
    }

    $selectors = explode(',', trim($selector));
    foreach ($selectors as $strSel) {
      if(array_key_exists('content', $rules_arr)) {
        $strSel = substr($strSel, 1, -7);
        $result[$strSel] = $strSel;
      }
    }
  }
  asort($result);
  return $result;
}
