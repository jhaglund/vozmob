<?php
// $Id: mms_us_verizon.inc,v 1.2 2007/10/28 13:36:08 stuartgreenfield Exp $

/**
 * Implementation of hook_mailsave_clean().
 */
function mms_us_verizon_mailsave_clean(&$node, $result, $i, $header, $mailbox) {

  // Build regex to see if this was from vodafone
  $pattern = '@vzwpix.com$@';
  if (preg_match($pattern, $header->fromaddress)) {

    // Build a regex to extract message Verizon spam from message.
    $node->body = preg_replace('/This message was sent using .* or higher is required\./s', '', $node->body);
    $node->body = preg_replace('/This message was sent using .* email, QuickTime/s', '', $node->body);

    // Strip surplus white space
    $node->body = preg_replace('@\s\s+@', ' ', $node->body);

    $node->teaser = node_teaser($node->body);

    // Set node title and reset the body if the author just puts short text in the body
    if (empty($node->title) && !empty($node->body) && strlen($node->body) <= 60) {
      $node->title = truncate_utf8($body, 60, TRUE, TRUE);
      $node->body = '';
    }

    // Get rid of extra text attachments
    foreach ($node->mailsave_attachments as $key => $file) {
      if ($file['filemime'] == 'text/plain') {
        unset($node->mailsave_attachments[$key]);
      }
    }

  }

  return;
}


/**
 * Report back module information when requested
 */
function mms_us_verizon_mailsave_clean_info() {
  $items['mms_us_verizon'] = array(
    'provider'    => t('Verizon'),
    'country'     => 'US',
    'author'      => 'Stuart Greenfield',
    'description' => t('Clean up MMS messages sent via the Verizon US network.'),
  );

  return $items;
}
