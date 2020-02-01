<?php


function commerce_oz_payway_admin_settings($form, &$form_state) {
	
	$form = array();
	
	/* Server Hosted Credentials Settings */
	
	$settings = _commweb_get_var('commerce_oz_gateway_payway');

	$logopath = '/' . drupal_get_path('module', 'commerce_oz') . '/image/payway_68.png';
	
	$descText = '<a href="http://www.payway.com.au"><img src="' . $logopath . '" /></a><br />Settings for the <a href="http://www.payway.com.au"/">PayWay</a> payment gateway.<br />';

  $form['intro_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('PayWay'),
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($descText),
  );


   /* Merchant Credentials Settings */

  $form['payway_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('PayWay API Credentials'),
    '#collapsible' => TRUE, 
  	'#collapsed' => TRUE,
    '#description' => t('Settings for your Payway Merchant Account. These are different to the login details for your PayWay Console.'),
  );
  
  $form['payway_settings']['payway_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Customer Username'),
    '#size' => 24,
    '#description' => t('.'),
    '#default_value' => $settings['payway_username'],
  );
  
  $form['payway_settings']['payway_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Customer Password'),
    '#size' => 70,
    '#description' => t('.'),
    '#default_value' => $settings['payway_password'],
  );

  $form['payway_settings']['payway_merchant'] = array(
    '#type' => 'textfield',
    '#title' => t('PayWay Merchant ID'),
    '#size' => 70,
    '#description' => t('Set this up in the PayWay portal.'),
    '#default_value' => $settings['payway_merchant'],
  );

  $form['payway_settings']['certificate_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Certificate Path'),
    '#size' => 90,
    '#description' => t('Relative path to your API Certificate.'),
    '#default_value' => isset($settings['certificate_path']) ? $settings['certificate_path'] : '' ,
  );

 
	/* Transaction Settings */

  $form['commerce_oz_mode_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Transaction Modes'),
    '#collapsible' => TRUE, 
  	'#collapsed' => TRUE,
    '#description' => t('Transaction Mode settings'),
  );
    
  
  // Live Mode
  $form['commerce_oz_mode_settings']['commerce_oz_transaction_mode'] = array(
    '#type' => 'select',
    '#title' => t('Transaction mode'),
    '#description' => t('Transaction mode used when sending requests. In Test mode you can adjust the Amount charged for testing purposes'),
    '#options' => array(
      'test' => t('Test'),
      'live' => t('Live'),
    ),
    '#default_value' => $settings['commerce_oz_transaction_mode'],
  );

	// Debug Mode
  $form['commerce_oz_mode_settings']['commerce_oz_debug_mode'] = array(
    '#type' => 'select',
    '#title' => t('Debug mode'),
    '#description' => t('Debug mode for recording requests and responses. In Debug mode all transactions are written to watchdog. Details and Merchant Account details are obfuscated.'),
    '#options' => array(
      '1' => t('Debug'),
      '0' => t('No Debug'),
    ),
    '#default_value' => $settings['commerce_oz_debug_mode'],

  );
  

	// Load Global Settings to the form
  module_load_include('inc',  'commerce_oz',  'includes/commerce_oz.global');
  $form += commerce_oz_global_admin_settings_form($settings);
  
  $form['commerce_oz_submit'] = array(
   
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
    
  );
  
  return $form;

}

function commerce_oz_payway_admin_settings_validate($form, &$form_state)
{
	
	return true;
}

function commerce_oz_payway_admin_settings_submit($form, &$form_state)
{
 	
 	$settings = $form_state['values'];
 	unset($settings['form_build_id']);
 	unset($settings['form_token']);
 	unset($settings['form_id']);
 	unset($settings['op']);
 	unset($settings['commerce_oz_submit']);
 	
	_commweb_set_var('commerce_oz_gateway_payway', $settings);
	
}


