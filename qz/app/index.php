<?php
define('APP_ROOT', __DIR__);
require_once('util/autoLoading.inc');

require_once('inc/env.inc');

header('Content-type: application/json');
function execute() {
	$act = 'Error';
	if (isset($_REQUEST['act'])) {
		$act = ucfirst($_REQUEST['act']);
	}
	$ref = new ReflectionClass($act.'Action');
	$inst = $ref->newInstance();
	$inst->execute();
}
execute();
?>
