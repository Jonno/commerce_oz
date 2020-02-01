<?php


function commerce_oz_cba_admin_settings($form, &$form_state) {
	
	$form = array();
	
	// Laod the decrypted settings array;
	$settings = _commweb_get_var('commerce_oz_gateway_cba');

	$logopath = '/' . drupal_get_path('module', 'commerce_oz') . '/image/cba_68.png';
	
	$descText = '<a href="http://www.commbank.com.au"><img src="' . $logopath . '"  align=left /></a>&nbsp;Settings for the <a href="http://www.commbank.com.au/business/merchant-services/payment-options/payments-via-your-website/default.aspx">CommWeb Virtual Payment Client</a> service provided by <a href="http://www.commbank.com.au">Commonwealth Bank</a> (Australia). It provides both Merchant Hosted Payment (2 Party) & Server Hosted (3 Party) for processing online credit cards payments.<p>For your protection, this software does not stored any credit card details on your servers. All credit card information is handed directly to the Payment Services</p> <p>Fill in the appropriate information below and ensure you have SSL enabled for the site.</p>';

  $form['commweb_intro_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('CommWeb VPC'),
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => t($descText),
  );


  $form['commweb_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('CommWeb Merchant Credentials'),
    '#collapsible' => TRUE, 
  	'#collapsed' => TRUE,
    '#description' => t('Settings related to your Commonwealth Bank Merchant Account.'),
  );
  
  $form['commweb_settings']['merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t('VPC Merchant ID'),
    '#size' => 24,
    '#description' => t('Your CommWeb Merchant ID as supplied to you by Commonwealth Bank, eg: "TESTABC0001".'),
    '#default_value' => $settings['merchant_id'],
  );
  
  $form['commweb_settings']['access_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Access Code'),
    '#size' => 12,
    '#description' => t('Your Access Code for making CommWeb VPC requests. You obtain this code from your CommWeb Merchant Portal'),
    '#default_value' => $settings['access_code'],
  );
 
   // Server Hosted (3P) elements  
 
  $form['commweb_3p'] = array(
    '#type' => 'fieldset',
    '#title' => t('Server Hosted Payments'),
    '#collapsible' => TRUE, 
  	'#collapsed' => TRUE,
    '#description' => t('Additional settings required for using CommWeb Server Hosted (3 Party) payment solutions.'),
  );
 
  $form['commweb_3p']['secure_secret_hash'] = array(
    '#type' => 'textfield',
    '#title' => t('Secure Secret Hash'),
    '#size' => 45,
    '#maxsize' => 32,
    '#description' => t('Your CommWeb Secure Secret Hash obtained from your CommWeb Merchant Portal.'),
    '#default_value' => $settings['secure_secret_hash'],
  );

  $form['commweb_3p']['3d_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('3DS - Verified by Visa'),
    '#collapsible' => TRUE, 
  	'#collapsed' => TRUE,
    '#description' => t("Settings required for using 3DS - 'Verified by Visa' with CommWeb Server Hosted (3 Party) payment solutions."),
  );


  $form['commweb_3p']['avs_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('AVS - Address Verification Service'),
    '#collapsible' => TRUE, 
  	'#collapsed' => TRUE,
    '#description' => t("Settings required for using AVS - 'Address Verification Service' with CommWeb Server Hosted (3 Party) payment solutions."),
  );


  // Refund elements  
 
  $form['commweb_refunds'] = array(
    '#type' => 'fieldset',
    '#title' => t('Refund Settings'),
    '#collapsible' => TRUE, 
  	'#collapsed' => TRUE,
    '#description' => t('Refund settings for your Commonwealth Bank VPC Account.<br />You need to enter the credentials of a user you created in your CommWeb Merchant portal to handle refunds.'),
  );
 
  $form['commweb_refunds']['refund_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant Refund User'),
    '#size' => 18,
    '#description' => t('CommWeb Merchant User making Refund requests.'),
    '#default_value' => $settings['refund_username'],
  );
  
  $form['commweb_refunds']['refund_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Passcode'),
    '#size' => 12,
    '#description' => t('Passcode for Merchant User making Refund requests.'),
    '#default_value' => $settings['refund_password'],
  );
  
  
    // Deafult Values for Test Cards  
  
  
  // New Field Set
  $form['commerce_oz_default_card'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default credit card values'),
    '#collapsible' => TRUE, 
  	'#collapsed' => TRUE,
    '#description' => t('These default values are only used for card elements when the module is in Test Mode. Useful for site building and development.'),
  );
  
  $form['commerce_oz_default_card']['commerce_oz_test_card_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Card Number'),
    '#description' => t('A default Credit Card Number'),
    '#size' => 20, 
  	'#maxlength' => 16, 
    '#default_value' => $settings['commerce_oz_test_card_number'],
    //$settings['commerce_commweb_default_card_settings']['commerce_commweb_test_card_number'],
  );

  $form['commerce_oz_default_card']['commerce_oz_test_card_exp_month'] = array(
    '#type' => 'select',
    '#title' => t('Expiry Month'),
    '#description' => t('A default Month (05)'),
    '#options' => drupal_map_assoc(array_keys(commerce_months())), 
   // '#size' => 3, 
  	//'#maxlength' => 2, 
  	 
    '#prefix' => '<div class="commerce-credit-card-expiration">', 
    '#suffix' => '<span class="commerce-month-year-divider">-</span>',
    '#default_value' => $settings['commerce_oz_test_card_exp_month'],
    //$settings['commerce_commweb_default_card_settings']['commerce_commweb_test_card_exp_month'],
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
    // $settings['commerce_commweb_default_card_settings']['commerce_commweb_test_card_exp_year'],
    '#suffix' => '</div>',
  );

  $form['commerce_oz_default_card']['commerce_oz_test_card_cvv'] = array(
    '#type' => 'textfield',
    '#title' => t('CVV Code'),
    '#description' => t('A default CVV Code'),
    '#size' => 4, 
  	'#maxlength' => 3, 
    '#default_value' => $settings['commerce_oz_test_card_cvv'],

   // $settings['commerce_commweb_default_card_settings']['commerce_commweb_test_card_cvv'],
  );




  // Data  capture elements  
  
  // Field Set
 
 
  $form['commerce_oz_capture_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Data Capture & Form Setup'),
    '#collapsible' => TRUE, 
  	'#collapsed' => TRUE,
    '#description' => t('Data capture settings for your Commonwealth Bank VPC Account'),
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
    '#description' => t('Select whether you want to collect and store the name of the card holder in your Order log. This value is not used (or required) by CommWeb.'),
    '#options' => array(
      'name' => t('Collect Card Holder Name'),
      'noname' => t('No Card Holder Name Required'),
    ),
    '#default_value' => $settings['commerce_oz_use_card_name'],
   
    
  );

	// Card Type

   $form['commerce_oz_capture_settings']['commerce_oz_select_card_type'] = array(
    '#type' => 'select',
    '#title' => t('Card Type'),
    '#description' => t('Select whether you want to collect and store the type of the card in your Order log. This value is not used (or required) by CommWeb.'),
    '#options' => array(
      'cardtype' => t('Collect Card Type '),
      'nocardtype' => t('No Card Type Required'),
    ),
    '#default_value' => $settings['commerce_oz_select_card_type'],
   
    
  );
  

     // Logo
    
   $form['commerce_oz_capture_settings']['commerce_commweb_vpc_show_logo'] = array(
    '#type' => 'select',
    '#title' => t('Secure CommWeb Logo'),
    '#description' => 'Show or hide the Commonwealth Bank "Online Secure Payment" logo.', 
    //<br /><img src="' . $secure_logo_path . '" /></a><br />',
    '#options' => array(
      'logo' => t('Show Logo'),
      'nologo' => t('Hide Logo'),
    ),
    '#default_value' => $settings['commerce_commweb_vpc_show_logo'],

  ); 


  $form['commerce_oz_mode_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Transaction Modes'),
    '#collapsible' => TRUE, 
  	'#collapsed' => TRUE,
    '#description' => t('Transaction Modes related to using your Commonwealth Bank Merchant Account.'),
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

  $form['commerce_oz_mode_settings']['commerce_oz_debug_mode'] = array(
    '#type' => 'select',
    '#title' => t('Debug mode'),
    '#description' => t('Debug mode for recording VPC requests and responses. In Debug mode all transactions are written to watchdog. Credit Card details and Merchant Account details are obfuscated.'),
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

function commerce_oz_cba_admin_settings_validate($form, &$form_state)
{
	
	
}

function commerce_oz_cba_admin_settings_submit($form, &$form_state)
{
	 	// dsm($form_state, 'form state');
	// 	// dsm($form, 'form');
 	
 	$settings = $form_state['values'];
 	unset($settings['form_build_id']);
 	unset($settings['form_token']);
 	unset($settings['form_id']);
 	unset($settings['op']);
 	unset($settings['commerce_oz_submit']);
 	
	_commweb_set_var('commerce_oz_gateway_cba', $settings);
	
}






