<?php
class ErrorAction extends ActionBase {
	function process() {
		$result = array('cause'=>'error.');
		echo json_encode($result);
	}
}
