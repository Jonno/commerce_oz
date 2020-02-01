<?php

/**
 * @file
 * Rules integration for Commerce Oz.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_oz_rules_event_info() {
  $events = array();

  $events['commerce_oz_transaction_complete'] = array(
    'label' => t('Complete a Commerce Oz Transaction'),
    'group' => t('Commerce Oz'),
    
    'variables' => array(
    
      'commerce_payment_transaction' => array(
        'type' => 'commerce_payment_transaction',
        'label' => t('Payment Transaction', array(), array('context' => 'a drupal commerce oz payment')),
      ),
			
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Completed order', array(), array('context' => 'a drupal commerce oz order')),
      ),
      
    ),
  //  'access callback' => 'commerce_order_rules_access',
  );

  $events['commerce_oz_transaction_success'] = array(
    'label' => t('A Commerce Oz Transaction Succeeded'),
    'group' => t('Commerce Oz'),
    'variables' => array(
      'commerce_payment_transaction' => array(
        'type' => 'commerce_payment_transaction',
        'label' => t('Payment Transaction', array(), array('context' => 'a drupal commerce oz payment')),
      ),

    ),
   // 'access callback' => 'commerce_order_rules_access',
  );

  $events['commerce_oz_transaction_fail'] = array(
    'label' => t('A Commerce Oz Transaction Failed'),
    'group' => t('Commerce Oz'),
    'variables' => array(
      'commerce_payment_transaction' => array(
        'type' => 'commerce_payment_transaction',
        'label' => t('Payment Transaction', array(), array('context' => 'a drupal commerce oz payment')),
      ),

    ),
   // 'access callback' => 'commerce_order_rules_access',
  );

  return $events;
}

/**
 * Implements hook_rules_condition_info().
 */
function commerce_oz_rules_condition_info() {
  $conditions = array();

  $conditions['commerce_oz_transaction_is_approved'] = array(
    'label' => t('Commerce Oz Transaction is Approved '),
    'parameter' => array(
      'commerce_payment_transaction' => array(
        'type' => 'commerce_payment_transaction',
        'label' => t('Payment Transaction'),
      ),
    ),
    'group' => t('Commerce Oz'),
    'callbacks' => array(
      'execute' => 'commerce_oz_rules_payment_is_accepted',
    ),
  );

  return $conditions;
}

/**
 * Rules condition: checks to see if the given transaction is accepted.
 */
function commerce_oz_rules_payment_is_accepted($transaction) {
  return true; // commerce_cart_order_is_cart($order);
}