<?php

/**
 *  BlockFanciblock extends Block
 *  
 *  This class allows us to create a block, set up the administrative interface 
 *  in Layout builder and set up configuration.
 */
class BlockFanciblock extends Block {
  /**
   *  Sets title text on draggable block panel in Layout builder.
   *  
   *  Defaults to the 'title' attribute set in hook_block_info
   */
  function getAdminTitle() {
    return t('Fanciblock');
  }

  /**
   *  Sets description text on draggable block in Layout builder.
   *  
   *  Defaults to the 'description' attribute set in hook_block_info
   */
  function getAdminPreview() {
    return t('Fanciblock.');
  }

  /**
   *  Sets block subject on block view.
   */
  function getTitle() {
    return t('Fanciblock)');
  }

  /**
   *  Sets block content on block view.
   */
  function getContent() {
    $content = !empty($this->settings['custom_class_string']) ? $this->settings['custom_class_string'] : 'No content saved';
    return $content;
  }

  /**
   *  Builds the block's configuration form.
   */
  function form(&$form, &$form_state) {
    module_load_include('inc', 'fanciblock', 'fanciblock.admin'); 
    $form += fanciblock_image_edit_box('edit');
    //unset($form_state['submit_handlers']['fanciblock_image_edit_box_submit']);
    //dd($form_state);
  }
  
  /**
   * Submit handler to save the form settings.
   *
   * Settings are saved to the active layout configuration file.
   */
  function formSubmit($form, &$form_state) {
    //parent::formSubmit($form, $form_state);
    dpm($this->toArray());
    dpm($form_state);
  }
}
/**
 * @} End of "defgroup block_example".
 */
