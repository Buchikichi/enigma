<?php
/**
 * 認証.
 * @author H.Sasai
 */
class AuthAction extends ActionBase {
	function getParamNames() {
		return array();
	}
	function makeData() {
		$user = $_SERVER['REMOTE_USER'];
		$dao = new UserDao();
		$rec = $dao->getRecord($user);
		if ($rec == null) {
			$this->cause = 'Bad user.';
			return null;
		}
		$now = date('Y-m-d H:i:s');
		return array(
			'ticket'=>$rec['userID'],
			'SERVER_NAME'=>SERVER_NAME,
			'sysDate'=>$now,
		);
	}
	function success($result) {
		return $result;
	}
}
