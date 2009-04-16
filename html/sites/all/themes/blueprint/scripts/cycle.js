// $Id$

$(document).ready(function() {
  // Set up the slideshow.
  var cycleIndex = 0;
  $('div.view-overlay div.views-field-field-image-fid span.field-content').each(function() {
    $(this).before('<div class="cycle-nav" id="cycle-nav-' + cycleIndex + '"></div>').cycle({ 
      fx:    'fade',
      pager: '#cycle-nav-' + cycleIndex,
      pause: 1
    });
    cycleIndex = 1 + cycleIndex;
  });
  // Set up the overlay and expose behaviors.
  $('div.field-field-image div.field-items a').each(function() {
    // Get the nid for this image field and set the rel attribute to the overlay for this node.
    var nid = $(this).parents('div.node').attr('id').slice(5);
    $(this).attr('rel', '#overlay-' + nid);
    $(this).attr('href', '#overlay-' + nid);
  });
  $('div.field-field-image div.field-items a[rel]').each(function() {
    $(this).overlay({
      onBeforeLoad: function() {
        this.getBackgroundImage().expose({color: '#000'});
      },
      onClose: function() {
        $.expose.close();
      }
    });
  });
});
