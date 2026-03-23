(function ($) {

"use strict";

Backdrop.behaviors.fanciblockDisplay = {
  attach: function(context) {
  $('.fanciblock').once('fanciblock', function() {
    $(this).each(function() {

      /*--- get max icon width ----*/
      let fa_icon_size = 0;
      $(this).find(".fanciblock-icon img, .fanciblock-icon .fa").each(function(){
        let fa_width = parseInt($(this).width());
        if (fa_width > fa_icon_size) {
          fa_icon_size = fa_width;
        }
      });
      let box_padding = $(this).find(".fanciblock-box").css("padding-left");
      // Content padding will approximate 10 as box_padding approaches 0.
      let padding_corrector = parseInt(box_padding) + (10/((100*parseInt(box_padding))+1));

      if($(this).find(".fanciblock-icon_size").length>0 && $(this).find(".fanciblock-icon_style-none").length==0){
      fa_icon_size = fa_icon_size+(fa_icon_size/2);
      }

      let fa_icon_pos_left = $(this).find(".fanciblock-icon_position-left");
      let fa_icon_pos_right = $(this).find(".fanciblock-icon_position-right");
      let fa_icon_pos_boxcut = $(this).find(".fanciblock-icon_position-boxcut");

      if(fa_icon_pos_left.length>0){
      $(this).find( ".fanciblock-title-content" ).css({'margin-left': fa_icon_size, 'padding-left': padding_corrector});
      $(this).find(".fanciblock-icon").css({top: box_padding, left: box_padding, position:'absolute'});
      }
      if(fa_icon_pos_right.length>0){
      $(this).find( ".fanciblock-title-content" ).css({'margin-right': fa_icon_size, 'padding-right': padding_corrector});
      $(this).find(".fanciblock-icon").css({top: box_padding, right: box_padding, position:'absolute'});
      }
      if(fa_icon_pos_boxcut.length>0){
        padding_top = parseInt($(this).find(".fanciblock-box").css('padding-top'));
      $(this).find(".fanciblock-box").css('margin-top', (fa_icon_size/2));
      $(this).find(".fanciblock-icon").css('margin-top', -(padding_top + fa_icon_size/2));
      }

      /*--- get max fanciblock box height ----*/
      let box_height = 0;
      $(this).find(".fanciblock-box").each(function(){
        let img_height = parseInt($(this).find('.fanciblock-icon').height());
        if (img_height > box_height) {
          box_height = img_height;
        }
        
      });
      $(this).find(".fanciblock-icon").height(box_height);

    }); 
  });
}};

})(jQuery);
