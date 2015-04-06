<?php
/**
 * 認証.
 * @author H.Sasai
 */
class ListAction extends ActionBase {
	function getParamNames() {
		return array('ticket');
	}
	function makeData() {
		$dao = new CategoryDao();
		$param = array(
			'from'=>0,
			'to'=>100,
		);
		$list = $dao->getList($param);
		$now = date('Y-m-d H:i:s');
		return array(
			'list'=>$list,
			'sysDate'=>$now,
		);
	}
	function success($result) {
		return $result;
	}
}
