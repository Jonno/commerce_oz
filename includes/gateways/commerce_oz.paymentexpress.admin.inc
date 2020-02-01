<?php


function commerce_oz_paymentexpress_admin_settings($form, &$form_state) {
	
	$form = array();
	
	/* Server Hosted Credentials Settings */
	
	$settings = _commweb_get_var('commerce_oz_gateway_paymentexpress');

	$logopath = '/' . drupal_get_path('module', 'commerce_oz') . '/image/paymentexpress_68.png';
	
	$descText = '<a href="http://www.paymentexpress.com"><img src="' . $logopath . '" /></a><br />Settings for the <a href="http://www.paymentexpress.com/">Payment Express</a> payment gateway.<br />Payment Express provides <a href="http://www.paymentexpress.com/Products/Ecommerce/Merchant_Hosted">PXPost for Merchant Hosted payments</a> (2 Party) & <a href="http://www.paymentexpress.com/Products/Ecommerce/DPS_Hosted">PXPay for Server Hosted</a> (3 Party) solutions for processing online credit card payments.<br />For your protection, this software does not store any credit card details on your server. All credit card information is handed securely to the Payment Express services.<br />Fill in the appropriate information below and ensure you have SSL enabled for the site.</p>';

  $form['intro_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('PaymentExpress'),
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($descText),
  );


   /* Merchant Hosted Credentials Settings */

  $form['pxpost_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('PXPost Credentials'),
    '#collapsible' => TRUE, 
  	'#collapsed' => TRUE,
    '#description' => t('Settings for your PaymentExpress PXPost Account. This are not the login details for your Payline Merchant portal.'),
  );
  
  $form['pxpost_settings']['pxpost_username'] = array(
    '#type' => 'textfield',
    '#title' => t('PXPost Username'),
    '#size' => 24,
    '#description' => t('Your PX Post Username as supplied to you by PaymentExpress.'),
    '#default_value' => $settings['pxpost_username'],
  );
  
  $form['pxpost_settings']['pxpost_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Access Code'),
    '#size' => 12,
    '#description' => t('Your PX Post Password for making transactions as supplied to you by PaymentExpress.'),
    '#default_value' => $settings['pxpost_password'],
  );
 
   /* Server Hosted Credentials Settings */ 
 
   $form['pxpay_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('PXPay Credentials'),
    '#collapsible' => TRUE, 
  	'#collapsed' => TRUE,
    '#description' => t('Settings for your PaymentExpress PXPay Account. This are not the login details for your Payline Merchant portal.'),
  );
  
  $form['pxpay_settings']['pxpay_username'] = array(
    '#type' => 'textfield',
    '#title' => t('PXPay Username'),
    '#size' => 24,
    '#description' => t('Your PXPay Username as supplied to you by PaymentExpress.'),
    '#default_value' => $settings['pxpay_username'],
  );
  
  $form['pxpay_settings']['pxpay_password'] = array(
    '#type' => 'textfield',
    '#title' => t('PXPay Access Key'),
    '#size' => 90,
    '#description' => t('Your PXPay Access Key for making transactions as supplied to you by PaymentExpress.'),
    '#default_value' => $settings['pxpay_password'],
  );
  
  $form['pxpay_settings']['3d_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('3DS - Verified by Visa'),
    '#collapsible' => TRUE, 
  	'#collapsed' => TRUE,
    '#description' => t("Settings required for using 3DS - 'Verified by Visa' with CommWeb Server Hosted (3 Party) payment solutions."),
  );


  $form['pxpay_settings']['avs_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('AVS - Address Verification Service'),
    '#collapsible' => TRUE, 
  	'#collapsed' => TRUE,
    '#description' => t("Settings required for using AVS - 'Address Verification Service' with CommWeb Server Hosted (3 Party) payment solutions."),
  );
 
 	
 	
	/* Default Values Settings */ 
  
  // New Field Set
  $form['commerce_oz_default_card'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default credit card values'),
    '#collapsible' => TRUE, 
  	'#collapsed' => TRUE,
    '#description' => t('These default values are only used for card elements when the module is in Test Mode. Useful for site building and development.'),
  );
 
  $form['commerce_oz_default_card']['commerce_oz_test_card_owner'] = array(
    '#type' => 'textfield',
    '#title' => t('Card Owner'),
    '#description' => t('A default Credit Card Owner'),
    '#size' => 40, 
  	'#maxlength' => 64, 
    '#default_value' => $settings['commerce_oz_test_card_owner'],
  );

  
  $form['commerce_oz_default_card']['commerce_oz_test_card_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Card Number'),
    '#description' => t('A default Credit Card Number'),
    '#size' => 20, 
  	'#maxlength' => 16, 
    '#default_value' => $settings['commerce_oz_test_card_number'],
  );

  $form['commerce_oz_default_card']['commerce_oz_test_card_exp_month'] = array(
    '#type' => 'select',
    '#title' => t('Expiry Month'),
    '#description' => t('A default Month (05)'),
    '#options' => drupal_map_assoc(array_keys(commerce_months())), 
    '#prefix' => '<div class="commerce-credit-card-expiration">', 
    '#suffix' => '<span class="commerce-month-year-divider">-</span>',
    '#default_value' => $settings['commerce_oz_test_card_exp_month'],
  );

	// Build a year select list that uses a 4 digit key with a 2 digit value.
	  $options = array();
	$current_year_2 = date('y');
  $current_year_4 = date('Y');
  

  for ($i = 0; $i < 20; $i++) {
    $options[$current_year_4 + $i] = str_pad($current_year_2 + $i, 2, '0', STR_PAD_LEFT);
  }

  $form['commerce_oz_default_card']['commerce_oz_test_card_exp_year'] = array(
    '#type' => 'select',
    '#title' => t('Expiry Year'),
    '#description' => t('A default year (2013)'),
    '#options' => $options, 
    '#default_value' => $settings['commerce_oz_test_card_exp_year'],
    '#suffix' => '</div>',
  );

  $form['commerce_oz_default_card']['commerce_oz_test_card_cvv'] = array(
    '#type' => 'textfield',
    '#title' => t('CVV Code'),
    '#description' => t('A default CVV Code'),
    '#size' => 4, 
  	'#maxlength' => 3, 
    '#default_value' => $settings['commerce_oz_test_card_cvv'],

  );




	/* Card form elements settings */ 
  
  // Field Set
 
 
  $form['commerce_oz_capture_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Credit Card Form Setup'),
    '#collapsible' => TRUE, 
  	'#collapsed' => TRUE,
    '#description' => t('Data capture settings for your Payment Express payment methods'),
  );
 

    
    // CVV
    
   $form['commerce_oz_capture_settings']['commerce_oz_use_cvv'] = array(
    '#type' => 'select',
    '#title' => t('CVV Code'),
    '#description' => t('Select whether you want to collect and check the CVV code from the rear of the card'),
    '#options' => array(
      'cvv' => t('Collect CVV'),
      'nocvv' => t('No CVV Required'),
    ),
    '#default_value' => $settings['commerce_oz_use_cvv'],
   
    
  );
	
	// Card Holder
	
   $form['commerce_oz_capture_settings']['commerce_oz_use_card_name'] = array(
    '#type' => 'select',
    '#title' => t('Card Holder Name'),
    '#description' => t('Select whether you want to collect and store the name of the card holder in your Order log. This value is not required by Payment Express.'),
    '#options' => array(
      'name' => t('Collect Card Holder Name'),
      'noname' => t('No Card Holder Name Required'),
    ),
    '#default_value' => $settings['commerce_oz_use_card_name'],
   
    
  );

  // Logo
    
   $form['commerce_oz_capture_settings']['commerce_oz_show_gateway_logo'] = array(
    '#type' => 'select',
    '#title' => t('Payment Express PXPost Logo'),
    '#description' => 'Show or hide the Payment Express logo on the credit card form. This only applies when using the PXPost payment method.', 
    '#options' => array(
      'logo' => t('Show Logo'),
      'nologo' => t('Hide Logo'),
    ),
    '#default_value' => $settings['commerce_oz_show_gateway_logo'],

  ); 

    // Privacy Links
    
   $form['commerce_oz_capture_settings']['commerce_oz_show_gateway_links'] = array(
    '#type' => 'select',
    '#title' => t('Privacy Statement Link'),
    '#description' => 'Show or hide the Payment Express Privacy Statement link on the credit card form. This only applies when using the PXPost payment method.', 
    '#options' => array(
      'link' => t('Show Link'),
      'nolink' => t('Hide Link'),
    ),
    '#default_value' => $settings['commerce_oz_show_gateway_links'],

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
    '#description' => t('Debug mode for recording requests and responses. In Debug mode all transactions are written to watchdog. Credit Card details and Merchant Account details are obfuscated.'),
    '#options' => array(
      '1' => t('Debug'),
      '0' => t('No Debug'),
    ),
    '#default_value' => $settings['commerce_oz_debug_mode'],

  );
  
  
  $form['commerce_oz_submit'] = array(
   
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
    
  );
  
  return $form;

}

function commerce_oz_paymentexpress_admin_settings_validate($form, &$form_state)
{
	
	return true;
}

function commerce_oz_paymentexpress_admin_settings_submit($form, &$form_state)
{
 	
 	$settings = $form_state['values'];
 	unset($settings['form_build_id']);
 	unset($settings['form_token']);
 	unset($settings['form_id']);
 	unset($settings['op']);
 	unset($settings['commerce_oz_submit']);
 	
	_commweb_set_var('commerce_oz_gateway_paymentexpress', $settings);
	
}






