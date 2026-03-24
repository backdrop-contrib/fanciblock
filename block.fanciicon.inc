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

    return !empty($this->settings['title']) ? check_plain($this->settings['title']) : t('Fanciicon');
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
    if (!empty($this->settings)) {
      return fanciblock_build_fanciblocks('fanciicon', $this->settings);
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
  }

  /**
   * Submit handler to save the form settings.
   */
  function formSubmit($form, &$form_state) {
    parent::formSubmit($form, $form_state);
    form_state_values_clean($form_state);
    $this->settings['boxes'] = $form_state['boxes'];
    $this->settings['fanciblock_name'] = $form_state['values']['fanciblock_name'];

    $values = _flatten_fanciblock_settings($form_state['values']);

    $this->settings['fanciblock_type'] = $values['fanciblock_type'];
    $this->settings['fanciblock_title'] = $values['fanciblock_title'];
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

    tempstore_clear('fanciblock', $this->settings['fanciblock_name']);

  }
}
