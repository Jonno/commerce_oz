<?php

/*
 * commerce_oz_admin_settings()
 *
 * Build the Admin form.
 */
function commerce_oz_admin_settings($form, &$form_state) {
	
	
//	commerce_oz_make_gateway_entity();
	
	
	// Initialise $form
	$form = array();
	
	// Initialise settings arrays
	$gateway_settings = commerce_oz_admin_init_settings(COMMERCE_OZ_GATEWAY_SETTINGS_VARIABLE, commerce_oz_gateway_providers());
	
	$method_settings = commerce_oz_admin_init_settings(COMMERCE_OZ_PAYMENT_METHOD_SETTINGS_VARIABLE, commerce_oz_commerce_payment_method_info());
	
	// // dsm($gateway_settings, '$gateway_settings');
	// // dsm($method_settings, '$method_settings');
	
  $descText = '<img src="' . commerce_oz_logo() . '" />' . COMMERCE_OZ_PRODUCT_NAME . t('provides a unified online Payment and Refund Management System to many major Australian eCommerce Gateways');

  $form['comm_oz_intro_settings'] = array(
    '#type' => 'fieldset',
    '#title' => COMMERCE_OZ_PRODUCT_NAME,
    '#collapsible' => FALSE, 
  	'#collapsed' => FALSE,
    '#description' => $descText,
  );


  $form['gateways'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gateway Selector'),
    '#collapsible' => TRUE, 
  	'#collapsed' => FALSE,
    '#description' => t('Enable the Payment Gateways you wish to use.<br />'),
  );

  $all_gateways = commerce_oz_gateway_providers();
 	// dsm($all_gateways, 'all_gateways');
  
  
  // Build Settings for each Gateway Enabled
  $headers = array(
  	'logo' => array('data' => t('Gateway')),
  	'gateway' => array('data' => t('Name')),
  	'payment_methods' => array('data' => t('Payment Methods')),
  	'settings' => array('data' => t('Settings')),
  );
  
  $default_values = array();

	foreach($all_gateways as $key => $gateway)
	{			// // dsm($gateway, '$gateway');
		
				$logo = '<img src ="' . commerce_oz_gateway_logo($gateway['base'], '_68') . '" />';
				
				$all_methods = commerce_oz_payment_methods_info();
				
				$methods = $all_methods[$gateway['base']];
				
				$description = '';
				
				foreach($methods as $method)
				{
					$description .= $method['admin_description'] . '<br />';
				}
	   	
		    $options[$gateway['base']] = array(
		    	'logo' => $logo,
		    	'gateway' => '<strong>' . $gateway['title'] . '</strong><br />Log on: <a href="' . $gateway['merchant_portal_login'] . '">' . $gateway['merchant_portal_name'] . '</a>',
		    	'payment_methods' => $description,
		    	'settings' => l(t('Edit settings'), 'admin/config/commerce_oz/gateways/'  .  $gateway['base']),
		    );
		    	
		    $default_value[$gateway['base']] = $gateway_settings[$gateway['base']];
		}

	 $form['gateways']['gateway_select'] = array(
			  '#type' => 'tableselect',
	      '#header' => $headers,
	      '#options' => $options,
	      '#default_value' => $default_value,
	      '#empty' => t('No gateways available. '),
		);

 
   // Build array for each Gateway Enabled
   
 	$enabled_gateways = array();
		
	foreach($gateway_settings as $key => $value)
	{
		if($value)
	 	{
	  	$enabled_gateways[$value] = commerce_oz_load_gateway_provider($value);
	  }
	}
	
	// dsm($enabled_gateways, '$enabled_gateways');
	  
  $gateway = array();
  
	// Build Settings for each Gateway Enabled
	$headers = array(
  	'payment_method' => array('data' => t('Payment Method')),
		'gateway' => array('data' => t('Gateway')),
  	'terminal' => array('data' => t('Terminal Usage')),
  	'offsite' => array('data' => t('Offsite')),
	);
  	
	$options = array();
  $default_values = array();
  
  foreach($enabled_gateways as $gateway)
  {
  	
	  $payment_methods = commerce_oz_load_gateway_payment_methods($gateway['base']);
	 
	  foreach($payment_methods as $payment_method)
		{
		
		$settings_string = 'None';
		if(isset($gateway['settings_url']))
		{
			$settings_string = l('Edit', $gateway['settings_url']) ;
		}
		
		$options[$payment_method['base']] = array(
		  'gateway' => $gateway['title'],
		  'payment_method' => $payment_method['title'],
	  	'offsite' => $payment_method['offsite'] ? 'Yes' : 'No',
	  	'terminal' => $payment_method['terminal'] ? 'Yes' : 'No',
		);

				$default_values[$payment_method['base']] = isset($method_settings[$payment_method['base']]) ? $method_settings[$payment_method['base']] : NULL;
		
  }
 }
 
  	$form['gateways']['payment_methods'] = array(
			  '#type' => 'tableselect',
	      '#header' => $headers,
	      '#options' => $options,
	     	'#default_value' => $default_values,
	      '#empty' => t('No payment methods available. Please enable a Payment Gateway from the list above.'),
		);

  
  // Submit Buttons
  
  $form['commerce_oz_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Configuration'),
  );  
  
  $form['#submit'][] = 'commerce_oz_admin_settings_submit';
  
  return $form;

}



/*
 * commerce_oz_admin_pm_form()
 *
 * Build the Admin form for payment method selection
 */
function commerce_oz_admin_pm_form($form, &$form_state) {
	
	// Initialise $form
	$form = array();
	
	// Initialise settings arrays
	$gateway_settings = commerce_oz_admin_init_settings(COMMERCE_OZ_GATEWAY_SETTINGS_VARIABLE, commerce_oz_gateway_providers());
	
	$method_settings = commerce_oz_admin_init_settings(COMMERCE_OZ_PAYMENT_METHOD_SETTINGS_VARIABLE, commerce_oz_commerce_payment_method_info());
	
	// // dsm($gateway_settings, '$gateway_settings');
	// // dsm($method_settings, '$method_settings');
	
   // Build array for each Gateway Enabled
   
 	$enabled_gateways = array();
		
	foreach($gateway_settings as $key => $value)
	{
		if($value)
	 	{
	  	$enabled_gateways[$value] = commerce_oz_load_gateway_provider($value);
	  }
	}
	
	// dsm($enabled_gateways, '$enabled_gateways');
	  
  $gateway = array();
  
	// Build Settings for each Gateway Enabled
	$headers = array(
  	'payment_method' => array('data' => t('Payment Method')),
		'gateway' => array('data' => t('Gateway')),
  	'terminal' => array('data' => t('Terminal Usage')),
  	'offsite' => array('data' => t('Offsite')),
  	'settings' => array('data' => t('Settings')),
	);
  	
	$options = array();
  $default_values = array();
  
  foreach($enabled_gateways as $gateway)
  {
  	
	  $payment_methods = commerce_oz_load_gateway_payment_methods($gateway['base']);
	 
	  foreach($payment_methods as $payment_method)
		{
		
		$settings_string = 'None';
		if(isset($gateway['settings_url']))
		{
			$settings_string = l('Edit', $gateway['settings_url']) ;
		}
		
		$options[$payment_method['base']] = array(
		  'gateway' => $gateway['title'],
		  'payment_method' => $payment_method['title'],
	  	'offsite' => $payment_method['offsite'] ? 'Yes' : 'No',
	  	'terminal' => $payment_method['terminal'] ? 'Yes' : 'No',
	  	'settings' => $settings_string,
		);
		
		
	//	if($payment_method['base'])
		//{		
		//// dsm($payment_method);
		//// dsm($method_settings);

				$default_values[$payment_method['base']] = isset($method_settings[$payment_method['base']]) ? $method_settings[$payment_method['base']] : NULL;
	//	}
		
  }
 }
 
  	$form['gateways']['payment_methods'] = array(
			  '#type' => 'tableselect',
	      '#header' => $headers,
	      '#options' => $options,
	     	'#default_value' => $default_values,
	      '#empty' => t('No payment methods available. Please enable a Payment Gateway from the list above.'),
		);

 
  
  return $form;

}





















function commerce_oz_admin_settings_validate($form, &$form_state)
{
	return TRUE;
}



function commerce_oz_admin_settings_submit($form, &$form_state)
{
	// dsm($form_state, 'form state');
	// // dsm($form, 'form');
	
	$gateway_settings_array = $form_state['values']['gateway_select']; 
	$method_settings_array = $form_state['values']['payment_methods'];
	
	$g_settings = _commweb_get_var(COMMERCE_OZ_GATEWAY_SETTINGS_VARIABLE);
	$m_settings = _commweb_get_var(COMMERCE_OZ_PAYMENT_METHOD_SETTINGS_VARIABLE);

	$g_settings = array_merge($g_settings, $gateway_settings_array);
	$m_settings = array_merge($m_settings, $method_settings_array);

	_commweb_set_var(COMMERCE_OZ_GATEWAY_SETTINGS_VARIABLE, $gateway_settings_array);
	_commweb_set_var(COMMERCE_OZ_PAYMENT_METHOD_SETTINGS_VARIABLE, $method_settings_array);
	
	// Inform Drupal of the changes
	menu_rebuild();
	commerce_payment_methods_reset();
	_commerce_payment_default_rules_reset(module_list()); //(array('commerce_oz'));
}
