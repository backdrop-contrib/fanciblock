<?php

/**
 *  BlockHero extends BlockText
 *
 *  This class allows us to create hero blocks with image backgrounds.
 */
class Fanciicon extends Block {
  /**
   * {@inheritdoc}
   */
  function __construct($plugin_name, array $data) {
    parent::__construct($plugin_name, $data);

    $this->settings += array(
      'image' => '',
      'image_path' => '',
    );
  }

  /**
   *  Sets title text on draggable block panel in Layout builder.
   */
  function getAdminTitle() {
    if (!empty($this->settings['admin_label'])) {
      return check_plain($this->settings['admin_label']);
    }

    return !empty($this->settings['title']) ? check_plain($this->settings['title']) : t('fanciblock block');
  }

  /**
   * Returns the rendered content of this block.
   *
   * If there is no content, a non-breaking space is returned to ensure the
   * block is still displayed, with only the background image applied.
   *
   * @return string
   */
  function getContent() {
    if (!empty($this->settings['content'])) {
      return check_markup(token_replace($this->settings['content']), $this->settings['format'], '', TRUE);
    }
    elseif (!empty($this->settings['title']) || !empty($this->settings['image'])) {
      return '&nbsp;';
    }
  }

  /**
   *  Builds the block's configuration form.
   */
  function form(&$form, &$form_state) {
    parent::form($form, $form_state);
    form_load_include($form_state, 'inc', 'fanciblock', 'fanciblock.box.admin');
    form_load_include($form_state, 'inc', 'fanciblock', 'fanciblock.admin');
    $tempstore_settings = fanciblock_get_settings($this->settings['fanciblock_name']);
    $settings = $tempstore_settings ? $tempstore_settings : $this->settings;
    $form += fanciblock_edit_box($form, $form_state, 'edit', 'fanciicon', $settings);
  }

  /**
   * Validates the form settings.
   */
  function formValidate($form, &$form_state) {
    parent::formValidate($form, $form_state);

    // Check that one of title, text, or image, is provided.
    // if (empty($form_state['values']['image']) && empty($form_state['values']['title']) && empty($form_state['values']['content']['value'])) {
      // form_error($form['image'], t('Background image, a Display title, or Block content must be provided.'));
    // }
  }

  /**
   * Submit handler to save the form settings.
   */
  function formSubmit($form, &$form_state) {
    parent::formSubmit($form, $form_state);
    form_state_values_clean($form_state);
    $this->settings['boxes'] = $form_state['values']['boxes'];
    unset($form_state['values']['boxes']);

    $values = _flatten_fanciblock_settings($form_state['values']);
    dvm($form_state['values'], 'formSubmit1');
    dvm($values, 'formSubmit');

    $this->settings['fanciblock_type'] = $values['fanciblock_type'];
    $this->settings['fanciblock_title'] = $values['fanciblock_title'];
    $this->settings['fanciblock_name'] = $values['fanciblock_name'];
    $this->settings['grid'] = $values['grid'];
    $this->settings['icon_position'] = $values['icon_position'];
    $this->settings['icon_style'] = $values['icon_style'];
    $this->settings['icon_size'] = $values['icon_size'];
    $this->settings['box_padding'] = $values['box_padding'];
    $this->settings['icon_bg_color_status'] = $values['icon_bg_color_status'];
    $this->settings['icon_bg_color'] = $values['icon_bg_color'];
    $this->settings['icon_border_status'] = $values['icon_border_status'];
    $this->settings['icon_border_width'] = $values['icon_border_width'];
    $this->settings['icon_border_style'] = $values['icon_border_style'];
    $this->settings['icon_border_color'] = $values['icon_border_color'];
    $this->settings['box_bg_color_status'] = $values['box_bg_color_status'];
    $this->settings['box_bg_color'] = $values['box_bg_color'];
    $this->settings['box_border_status'] = $values['box_border_status'];
    $this->settings['box_border_width'] = $values['box_border_width'];
    $this->settings['box_border_style'] = $values['box_border_style'];
    $this->settings['box_border_color'] = $values['box_border_color'];
      
    dvm($this->settings, 'this->settings');

/*     // Save the things not handled by BlockText.
    if (!empty($form_state['values']['image'])) {
      $file = file_load($form_state['values']['image']);
      if ($form_state['values']['image'] != $form['image']['#default_value']) {
        // Record that the layout module is using this file. Also marks permanent.
        // But only if the value has changed.
        file_usage_add($file, 'layout', 'block', $file->fid);
        if (!empty($form['image']['#default_value'])) {
          // Decrement usage for replaced file.
          $old_file = file_load($form['image']['#default_value']);
          if ($old_file) {
            file_usage_delete($old_file, 'layout', 'block', $old_file->fid);
          }
        }
      }
      // Store the filepath separately for use as inline background image.
      $absolute_path = parse_url($GLOBALS['base_url'], PHP_URL_PATH) . '/';
      $fileurl = file_create_url($file->uri);
      $filepath = str_replace($GLOBALS['base_url'] . '/', $absolute_path, $fileurl);
      $form_state['values']['image_path'] = $filepath;
      $this->settings['image'] = $form_state['values']['image'];
      $this->settings['image_path'] = $form_state['values']['image_path'];
    }
    else {
      // No new file but an old one existed, decrement usage.
      if (!empty($form['image']['#default_value'])) {
        $old_file = file_load($form['image']['#default_value']);
        if ($old_file) {
          file_usage_delete($old_file, 'layout', 'block', $old_file->fid);
        }
      }
      $this->settings['image'] = NULL;
      $this->settings['image_path'] = NULL;
    }
 */  }
}
