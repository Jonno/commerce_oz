<?php

/**
 * @file
 * Rules integration for the checkout process.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_oz_refund_rules_event_info() {
  $events = array();

  $events['commerce_oz_refund_complete'] = array(
    'label' => t('Complete a Commerce Oz Refund'),
    'group' => t('Commerce Oz'),
    
    'variables' => array(
    
      'commerce_payment_transaction' => array(
        'type' => 'commerce_payment_transaction',
        'label' => t('Payment Transaction', array(), array('context' => 'a drupal commerce oz payment')),
      ),

    ),
    'access callback' => 'commerce_order_rules_access',
  );

  return $events;
}

/**
 * Implements hook_rules_condition_info().
 */
function commerce_oz_refund_rules_condition_info() {
  $conditions = array();

  $conditions['commerce_oz_refund_is_approved'] = array(
    'label' => t('Refund Transaction is Approved '),
    'parameter' => array(
      'commerce_payment_transaction' => array(
        'type' => 'commerce_payment_transaction',
        'label' => t('Payment Transaction'),
      ),
    ),
    'group' => t('Commerce Oz'),
    'callbacks' => array(
      'execute' => 'commerce_oz_refund_rules_payment_is_accepted',
    ),
  );

  return $conditions;
}

/**
 * Rules condition: checks to see if the given transaction is accepted.
 */
function commerce_oz_refund_rules_payment_is_accepted($transaction) {
  return $transaction->remote_id == 0; 
}





/**
 * Implements hook_rules_action_info().
 */
function commerce_oz_refund_rules_action_info() {
  $actions = array();

  $actions['send_account_email'] = array(
    'label' => t('Send account e-mail'),
    'parameter' => array(
      'account' => array(
        'type' => 'user',
        'label' => t('Account'),
      ),
      'email_type' => array(
        'type' => 'text',
        'label' => t('E-mail type'),
        'description' => t("Select the e-mail based on your site's account settings to send to the user."),
        'options list' => 'commerce_checkout_account_email_options_list',
      ),
    ),
    'group' => t('User'),
    'base' => 'commerce_checkout_action_send_account_email',
    'access callback' => 'rules_system_integration_access',
  );
  
  $actions['commerce_checkout_complete'] = array(
    'label' => t('Complete checkout for an order'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order in checkout'),
      ),
    ),
    'group' => t('Commerce Checkout'),
    'callbacks' => array(
      'execute' => 'commerce_checkout_rules_complete_checkout',
    ),
  );

  return $actions;
}
