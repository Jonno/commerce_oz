<?php


function commerce_oz_poli_admin_settings($form, &$form_state) {
	
	$form = array();
	
	/* Server Hosted Credentials Settings */
	
	$settings = _commweb_get_var('commerce_oz_gateway_poli');

	$logopath = '/' . drupal_get_path('module', 'commerce_oz') . '/image/poli_68.png';
	
	$descText = '<a href="http://www.polipayments.com"><img src="' . $logopath . '" /></a><br />Settings for the <a href="http://www.polipayments.com/">POLi Payments</a> payment gateway.<br />POLi Payments provides <a href="http://www.polipayments.com/consumer">a unique service</a> for handling customer payments without a credit card by utilising the customer\'s online banking service to effect a bank transfer to your account.<br />This software does not store any credit card details on your server.<br />Fill in the appropriate information below and ensure you have SSL enabled for the site.</p>';

  $form['intro_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('POLI Payments'),
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($descText),
  );


   /* Merchant Credentials Settings */

  $form['poli_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('POLi Credentials'),
    '#collapsible' => TRUE, 
  	'#collapsed' => TRUE,
    '#description' => t('Settings for your POLi Merchant Account. These are are different to the login details for your POLi Console.'),
  );
  
  $form['poli_settings']['poli_merchant'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant Code'),
    '#size' => 24,
    '#description' => t('Your POLi Merchant Code as supplied to you by POLi Payments.'),
    '#default_value' => $settings['poli_merchant'],
  );
  
  $form['poli_settings']['poli_auth_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Access Code'),
    '#size' => 70,
    '#description' => t('Your POLi Authentication Code for making transactions as set up in the POLi Console.'),
    '#default_value' => $settings['poli_auth_code'],
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
  

  $bl = _commerce_oz_poli_get_bank_list();
  
  dsm($bl);
  
  $form['commerce_oz_submit'] = array(
   
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
    
  );
  
  return $form;

}

function commerce_oz_poli_admin_settings_validate($form, &$form_state)
{
	
	return true;
}

function commerce_oz_poli_admin_settings_submit($form, &$form_state)
{
 	
 	$settings = $form_state['values'];
 	unset($settings['form_build_id']);
 	unset($settings['form_token']);
 	unset($settings['form_id']);
 	unset($settings['op']);
 	unset($settings['commerce_oz_submit']);
 	
	_commweb_set_var('commerce_oz_gateway_poli', $settings);
	
}


