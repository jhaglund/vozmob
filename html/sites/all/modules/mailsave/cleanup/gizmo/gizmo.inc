<?php
// $Id$

/**
 * @file
 * Cleanup messages received from Gizmo.
 */

/**
 * Implementation of hook_mailsave_clean().
 */
function gizmo_mailsave_clean(&$node, $result, $i, $header, $mailbox) {
  // Check if this message was from Gizmo.
  if (is_int(strpos($header->fromaddress, 'voicemail@gizmoproject.com'))) {
    // Check for "Voicemail from:" in case the format changes.
    // Worst case is that match fails and nothing is changed.
    if (is_int(strpos($node->body, '<title>You have a new voicemail message!</title>')) || is_int(strpos($node->body, '<title>Voicemail from:'))) {
      // Store the amended body
      $node->body = '';
      $node->teaser = node_teaser($node->body);
    }
    $node->title = t('Voice message');
    // For any attached WAV files, try to match the caller with an
    // existing user; remove phone number from filename and title.
    foreach ($node->mailsave_attachments as $index => $file) {
      $pattern = '/^[0-9]+/';
      if ($file['filemime'] == 'audio/x-wav' && preg_match($pattern, $file['filename'], $matches)) {
        if (module_exists('sms_user')) {
          sms_mailsave_receive($node, $matches[0]);
        }
        if ($node->uid) {
          mailhandler_switch_user($node->uid);
        }
        $node->mailsave_attachments[$index]['filename'] = preg_replace($pattern, 'unknown', $file['filename']);
      }
    }
  }
}

/**
 * Report back module information when requested.
 */
function gizmo_mailsave_clean_info() {
  $items['gizmo'] = array(
    'provider'    => t('Gizmo'),
    'country'     => 'US',
    'author'      => 'Mark Burdett',
    'description' => t('Clean up voice mail messages sent via the Gizmo service.'),
  );
  return $items;
}
