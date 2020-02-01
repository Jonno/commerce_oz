<?php
/**
 * @file
 * Provides default rules needed by the module
 * @copyright Copyright(c) 2011 John Oakley
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 */

/**
 * Implementation of hook_default_rules_configuration().
 */
function commerce_oz_refund_default_rules_configuration() {

 $rules = array();
  
   // Add a reaction rule to display the default Add to Cart message.
  $rule = rules_reaction_rule();

  $rule->label = t('Display a Refund Complete message');
  $rule->active = TRUE;

  $rule
    ->event('commerce_oz_refund_complete')
    ->action('drupal_message', array(
    'message' => 'A Commerce Oz Refund has been completed.
									Remote ID: [commerce-payment-transaction:remote-id]
									Message is: [commerce-payment-transaction:message]'));

  $rules['commerce_oz_refund_complete'] = $rule;
  
  return $rules;
}
