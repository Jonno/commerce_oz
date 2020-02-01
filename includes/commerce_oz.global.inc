<?php


function commerce_oz_global_admin_settings_form($settings = array()) {
	

  // Default Values for Test Cards  
  
  
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
    '#default_value' => isset($settings['commerce_oz_test_card_number']) ? $settings['commerce_oz_test_card_number'] : '',
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
    '#default_value' => isset($settings['commerce_oz_test_card_exp_month']) ? $settings['commerce_oz_test_card_exp_month'] : '',
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
    '#default_value' => isset($settings['commerce_oz_test_card_exp_year']) ? $settings['commerce_oz_test_card_exp_year'] : '',
    // $settings['commerce_commweb_default_card_settings']['commerce_commweb_test_card_exp_year'],
    '#suffix' => '</div>',
  );

  $form['commerce_oz_default_card']['commerce_oz_test_card_cvv'] = array(
    '#type' => 'textfield',
    '#title' => t('CVV Code'),
    '#description' => t('A default CVV Code'),
    '#size' => 4, 
  	'#maxlength' => 3, 
    '#default_value' => isset($settings['commerce_oz_test_card_cvv']) ? $settings['commerce_oz_test_card_cvv'] : '',

  );

  // Data  capture elements  
  
  // Field Set
 
 
  $form['commerce_oz_capture_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Form Setup'),
    '#collapsible' => TRUE, 
  	'#collapsed' => TRUE,
    '#description' => t('Data capture settings for your Merchant Account'),
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
    '#default_value' => isset($settings['commerce_oz_use_cvv']) ? $settings['commerce_oz_use_cvv'] : '',
   
    
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
    '#default_value' => isset($settings['commerce_oz_use_card_name']) ? $settings['commerce_oz_use_card_name'] : '',
   
    
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
    '#default_value' => isset($settings['commerce_oz_select_card_type']) ? $settings['commerce_oz_select_card_type'] : '',
   
    
  );
  

     // Logo
    
   $form['commerce_oz_capture_settings']['commerce_oz_show_form_logo'] = array(
    '#type' => 'select',
    '#title' => t('Show Gateway Logo'),
    '#description' => 'Show or hide the gateway\'s logo to users on the payment form.', 
    '#options' => array(
      'logo' => t('Show Logo'),
      'nologo' => t('Hide Logo'),
    ),
    '#default_value' => isset($settings['commerce_oz_show_form_logo']) ? $settings['commerce_oz_show_form_logo'] : '',

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
    '#default_value' => isset($settings['commerce_oz_transaction_mode']) ? $settings['commerce_oz_transaction_mode'] : '',
  );

  $form['commerce_oz_mode_settings']['commerce_oz_debug_mode'] = array(
    '#type' => 'select',
    '#title' => t('Debug mode'),
    '#description' => t('Debug mode for recording VPC requests and responses. In Debug mode all transactions are written to watchdog. Credit Card details and Merchant Account details are obfuscated.'),
    '#options' => array(
      '1' => t('Debug'),
      '0' => t('No Debug'),
    ),
    '#default_value' => isset($settings['commerce_oz_debug_mode']) ? $settings['commerce_oz_debug_mode'] : '',
  );
  
  return $form;

}







