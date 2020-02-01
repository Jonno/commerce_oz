<?php

/**
 * Export Drupal Commerce orders to Views.
 */

/**
 * Implements hook_views_data()
 */
function commerce_oz_test_views_data() {
  $data = array();

  // Define an area handler that displays a "CommWeb" Test form.
  $data['views']['commerce_oz_tests'] = array(
    'title' => t('Commerce Oz Tests'),
    'help' => t('Display a form that performs Tests on Commerce Oz Payment gateways.'),
    'area' => array(
      'handler' => 'commerce_oz_test_handler_area_view_order_form'
    ),
  );

  return $data;
}
