<?php

/**
 * Views for the CommWeb Refund UI.
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_oz_test_views_default() {

  $views = array();
  
	$view = new view();
	$view->name = 'commweb_tests';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'commerce_payment_transaction';
	$view->human_name = 'Commerce Oz Tests';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
	
	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'CommWeb Tests';
	$handler->display->display_options['use_ajax'] = TRUE;
	$handler->display->display_options['use_more_always'] = FALSE;
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['access']['perm'] = 'administer commerce_coz';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['exposed_form']['type'] = 'better_exposed_filters';
	$handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
	$handler->display->display_options['exposed_form']['options']['bef'] = array(
	  'general' => array(
	    'allow_secondary' => 0,
	    'secondary_label' => 'Advanced options',
	  ),
	  'sort' => array(
	    'bef_format' => 'default',
	    'advanced' => array(
	      'collapsible' => 0,
	      'collapsible_label' => 'Sort options',
	      'combine' => 0,
	      'combine_rewrite' => '',
	      'reset' => 1,
	      'reset_label' => '',
	      'is_secondary' => 0,
	    ),
	  ),
	  'status' => array(
	    'bef_format' => 'default',
	    'more_options' => array(
	      'bef_select_all_none' => FALSE,
	      'bef_collapsible' => 0,
	      'is_secondary' => 0,
	      'bef_filter_description' => '',
	      'tokens' => array(
	        'available' => array(
	          0 => 'global_types',
	        ),
	      ),
	    ),
	  ),
	);
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '20';
	$handler->display->display_options['pager']['options']['offset'] = '0';
	$handler->display->display_options['pager']['options']['id'] = '0';
	$handler->display->display_options['pager']['options']['quantity'] = '9';
	$handler->display->display_options['pager']['options']['expose']['items_per_page'] = TRUE;
	$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = TRUE;
	$handler->display->display_options['style_plugin'] = 'table';
	$handler->display->display_options['style_options']['columns'] = array(
	  'views_bulk_operations' => 'views_bulk_operations',
	  'status' => 'status',
	  'transaction_id' => 'transaction_id',
	  'amount' => 'amount',
	  'remote_id' => 'remote_id',
	  'remote_status' => 'remote_status',
	  'changed' => 'changed',
	  'message' => 'message',
	  'operations' => 'operations',
	  'order_id' => 'order_id',
	  'payment_method' => 'payment_method',
	);
	$handler->display->display_options['style_options']['default'] = 'transaction_id';
	$handler->display->display_options['style_options']['info'] = array(
	  'views_bulk_operations' => array(
	    'align' => '',
	    'separator' => '',
	    'empty_column' => 0,
	  ),
	  'status' => array(
	    'sortable' => 0,
	    'default_sort_order' => 'asc',
	    'align' => '',
	    'separator' => '',
	    'empty_column' => 0,
	  ),
	  'transaction_id' => array(
	    'sortable' => 1,
	    'default_sort_order' => 'desc',
	    'align' => '',
	    'separator' => '',
	    'empty_column' => 0,
	  ),
	  'amount' => array(
	    'sortable' => 0,
	    'default_sort_order' => 'asc',
	    'align' => '',
	    'separator' => '',
	    'empty_column' => 0,
	  ),
	  'remote_id' => array(
	    'sortable' => 0,
	    'default_sort_order' => 'asc',
	    'align' => '',
	    'separator' => '',
	    'empty_column' => 0,
	  ),
	  'remote_status' => array(
	    'sortable' => 0,
	    'default_sort_order' => 'asc',
	    'align' => '',
	    'separator' => '',
	    'empty_column' => 0,
	  ),
	  'changed' => array(
	    'sortable' => 0,
	    'default_sort_order' => 'asc',
	    'align' => '',
	    'separator' => '',
	    'empty_column' => 0,
	  ),
	  'message' => array(
	    'sortable' => 0,
	    'default_sort_order' => 'asc',
	    'align' => '',
	    'separator' => '',
	    'empty_column' => 0,
	  ),
	  'operations' => array(
	    'align' => '',
	    'separator' => '',
	    'empty_column' => 0,
	  ),
	  'order_id' => array(
	    'sortable' => 0,
	    'default_sort_order' => 'asc',
	    'align' => '',
	    'separator' => '',
	    'empty_column' => 0,
	  ),
	  'payment_method' => array(
	    'sortable' => 0,
	    'default_sort_order' => 'asc',
	    'align' => '',
	    'separator' => '',
	    'empty_column' => 0,
	  ),
	);
	/* Header: Global: Result summary */
	$handler->display->display_options['header']['result']['id'] = 'result';
	$handler->display->display_options['header']['result']['table'] = 'views';
	$handler->display->display_options['header']['result']['field'] = 'result';
	/* Field: Bulk operations: Commerce Payment Transaction */
	$handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
	$handler->display->display_options['fields']['views_bulk_operations']['table'] = 'commerce_payment_transaction';
	$handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
	$handler->display->display_options['fields']['views_bulk_operations']['label'] = 'CommWeb Transaction';
	$handler->display->display_options['fields']['views_bulk_operations']['vbo']['entity_load_capacity'] = '10';
	$handler->display->display_options['fields']['views_bulk_operations']['vbo']['operations'] = array(
	  'action::views_bulk_operations_delete_item' => array(
	    'selected' => 0,
	    'use_queue' => 0,
	    'skip_confirmation' => 0,
	    'override_label' => 0,
	    'label' => '',
	  ),
	  'action::commerce_commweb_delete_order_transaction_action' => array(
	    'selected' => 1,
	    'use_queue' => 0,
	    'skip_confirmation' => 0,
	    'override_label' => 0,
	    'label' => '',
	  ),
	  'action::system_message_action' => array(
	    'selected' => 0,
	    'use_queue' => 0,
	    'skip_confirmation' => 0,
	    'override_label' => 0,
	    'label' => '',
	  ),
	  'action::views_bulk_operations_script_action' => array(
	    'selected' => 0,
	    'use_queue' => 0,
	    'skip_confirmation' => 0,
	    'override_label' => 0,
	    'label' => '',
	  ),
	  'action::views_bulk_operations_modify_action' => array(
	    'selected' => 0,
	    'use_queue' => 0,
	    'skip_confirmation' => 0,
	    'override_label' => 0,
	    'label' => '',
	    'settings' => array(
	      'show_all_tokens' => 1,
	      'display_values' => array(
	        '_all_' => '_all_',
	      ),
	    ),
	  ),
	  'action::views_bulk_operations_argument_selector_action' => array(
	    'selected' => 0,
	    'skip_confirmation' => 0,
	    'override_label' => 0,
	    'label' => '',
	    'settings' => array(
	      'url' => '',
	    ),
	  ),
	  'action::twitter_actions_set_status_action' => array(
	    'selected' => 0,
	    'use_queue' => 0,
	    'skip_confirmation' => 0,
	    'override_label' => 0,
	    'label' => '',
	  ),
	  'action::system_goto_action' => array(
	    'selected' => 0,
	    'use_queue' => 0,
	    'skip_confirmation' => 0,
	    'override_label' => 0,
	    'label' => '',
	  ),
	  'action::commerce_commweb_refund_transaction_action' => array(
	    'selected' => 0,
	    'use_queue' => 0,
	    'skip_confirmation' => 1,
	    'override_label' => 0,
	    'label' => '',
	  ),
	  'action::system_send_email_action' => array(
	    'selected' => 0,
	    'use_queue' => 0,
	    'skip_confirmation' => 0,
	    'override_label' => 0,
	    'label' => '',
	  ),
	);
	$handler->display->display_options['fields']['views_bulk_operations']['vbo']['enable_select_all_pages'] = 1;
	$handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_type'] = '1';
	$handler->display->display_options['fields']['views_bulk_operations']['vbo']['display_result'] = 0;
	$handler->display->display_options['fields']['views_bulk_operations']['vbo']['merge_single_action'] = 0;
	$handler->display->display_options['fields']['views_bulk_operations']['vbo']['force_single'] = 0;
	/* Field: Commerce Payment Transaction: Status */
	$handler->display->display_options['fields']['status']['id'] = 'status';
	$handler->display->display_options['fields']['status']['table'] = 'commerce_payment_transaction';
	$handler->display->display_options['fields']['status']['field'] = 'status';
	/* Field: Commerce Payment Transaction: Transaction ID */
	$handler->display->display_options['fields']['transaction_id']['id'] = 'transaction_id';
	$handler->display->display_options['fields']['transaction_id']['table'] = 'commerce_payment_transaction';
	$handler->display->display_options['fields']['transaction_id']['field'] = 'transaction_id';
	$handler->display->display_options['fields']['transaction_id']['label'] = 'ID';
	/* Field: Commerce Payment Transaction: Amount */
	$handler->display->display_options['fields']['amount']['id'] = 'amount';
	$handler->display->display_options['fields']['amount']['table'] = 'commerce_payment_transaction';
	$handler->display->display_options['fields']['amount']['field'] = 'amount';
	/* Field: Commerce Payment Transaction: Remote ID */
	$handler->display->display_options['fields']['remote_id']['id'] = 'remote_id';
	$handler->display->display_options['fields']['remote_id']['table'] = 'commerce_payment_transaction';
	$handler->display->display_options['fields']['remote_id']['field'] = 'remote_id';
	/* Field: Commerce Payment Transaction: Remote status */
	$handler->display->display_options['fields']['remote_status']['id'] = 'remote_status';
	$handler->display->display_options['fields']['remote_status']['table'] = 'commerce_payment_transaction';
	$handler->display->display_options['fields']['remote_status']['field'] = 'remote_status';
	/* Field: Commerce Payment Transaction: Changed date */
	$handler->display->display_options['fields']['changed']['id'] = 'changed';
	$handler->display->display_options['fields']['changed']['table'] = 'commerce_payment_transaction';
	$handler->display->display_options['fields']['changed']['field'] = 'changed';
	$handler->display->display_options['fields']['changed']['label'] = 'Changed';
	$handler->display->display_options['fields']['changed']['date_format'] = 'short';
	/* Field: Commerce Payment Transaction: Message */
	$handler->display->display_options['fields']['message']['id'] = 'message';
	$handler->display->display_options['fields']['message']['table'] = 'commerce_payment_transaction';
	$handler->display->display_options['fields']['message']['field'] = 'message';
	/* Field: Commerce Payment Transaction: Operations links */
	$handler->display->display_options['fields']['operations']['id'] = 'operations';
	$handler->display->display_options['fields']['operations']['table'] = 'commerce_payment_transaction';
	$handler->display->display_options['fields']['operations']['field'] = 'operations';
	$handler->display->display_options['fields']['operations']['label'] = 'Action';
	/* Field: Commerce Payment Transaction: Order ID */
	$handler->display->display_options['fields']['order_id']['id'] = 'order_id';
	$handler->display->display_options['fields']['order_id']['table'] = 'commerce_payment_transaction';
	$handler->display->display_options['fields']['order_id']['field'] = 'order_id';
	$handler->display->display_options['fields']['order_id']['link_to_order'] = 'admin';
	/* Field: Commerce Payment Transaction: Payment method */
	$handler->display->display_options['fields']['payment_method']['id'] = 'payment_method';
	$handler->display->display_options['fields']['payment_method']['table'] = 'commerce_payment_transaction';
	$handler->display->display_options['fields']['payment_method']['field'] = 'payment_method';
	$handler->display->display_options['fields']['payment_method']['label'] = 'Method';
	/* Field: Commerce Order: Delete link */
	$handler->display->display_options['fields']['delete_order']['id'] = 'delete_order';
	$handler->display->display_options['fields']['delete_order']['table'] = 'commerce_order';
	$handler->display->display_options['fields']['delete_order']['field'] = 'delete_order';
	$handler->display->display_options['fields']['delete_order']['label'] = 'Delete Order';
	$handler->display->display_options['fields']['delete_order']['text'] = 'Delete';
	/* Sort criterion: Commerce Payment Transaction: Transaction ID */
	$handler->display->display_options['sorts']['transaction_id']['id'] = 'transaction_id';
	$handler->display->display_options['sorts']['transaction_id']['table'] = 'commerce_payment_transaction';
	$handler->display->display_options['sorts']['transaction_id']['field'] = 'transaction_id';
	$handler->display->display_options['sorts']['transaction_id']['order'] = 'DESC';
	$handler->display->display_options['sorts']['transaction_id']['exposed'] = TRUE;
	$handler->display->display_options['sorts']['transaction_id']['expose']['label'] = 'ID';
	/* Filter criterion: Commerce Payment Transaction: Payment method */
	$handler->display->display_options['filters']['payment_method']['id'] = 'payment_method';
	$handler->display->display_options['filters']['payment_method']['table'] = 'commerce_payment_transaction';
	$handler->display->display_options['filters']['payment_method']['field'] = 'payment_method';
	$handler->display->display_options['filters']['payment_method']['value'] = array(
	  'commerce_commweb' => 'commerce_commweb',
	);
	/* Filter criterion: Commerce Payment Transaction: Status */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'commerce_payment_transaction';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = array(
	  'all' => 'all',
	  'pending' => 'pending',
	  'success' => 'success',
	  'failure' => 'failure',
	);
	$handler->display->display_options['filters']['status']['exposed'] = TRUE;
	$handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
	$handler->display->display_options['filters']['status']['expose']['label'] = 'Status';
	$handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
	$handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
	$handler->display->display_options['filters']['status']['expose']['remember'] = TRUE;
	$handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
	  2 => 0,
	  3 => '3',
	  1 => 0,
	);
	$handler->display->display_options['filters']['status']['expose']['reduce'] = TRUE;
	/* Filter criterion: Commerce Payment Transaction: Message */
	$handler->display->display_options['filters']['message']['id'] = 'message';
	$handler->display->display_options['filters']['message']['table'] = 'commerce_payment_transaction';
	$handler->display->display_options['filters']['message']['field'] = 'message';
	$handler->display->display_options['filters']['message']['operator'] = 'contains';
	$handler->display->display_options['filters']['message']['value'] = 'TEST';
	
	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
	$handler->display->display_options['path'] = 'admin/config/development/commweb_tests2';
	$handler->display->display_options['menu']['type'] = 'normal';
	$handler->display->display_options['menu']['title'] = 'Transactions';
	$handler->display->display_options['menu']['weight'] = '0';
	$handler->display->display_options['menu']['name'] = 'main-menu';
	$handler->display->display_options['menu']['context'] = 0;



  $views[$view->name] = $view;

  return $views;
}
