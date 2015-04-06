<?php
/**
 * 開始.
 * @author H.Sasai
 */
class BeginAction extends ActionBase {
	function getParamNames() {
		return array('ticket', 'catID', 'numOfQuestion');
	}
	function makeData() {
		$catID = $_REQUEST['catID'];
		$numOfQuestion = intval($_REQUEST['numOfQuestion']);
		$questionDao = new QuestionDao();
		$cnt = $questionDao->count($catID);
		$numList = array();
		$giveup = 10000;

		while (0 < $giveup && count($numList) < $numOfQuestion) {
			$num = rand(0, $cnt - 1);
			if (!in_array($num, $numList)) {
				$numList[] = $num;
			}
			$giveup--;
		}
		$list = array();
		foreach ($numList as $num) {
			$rec = $questionDao->getQuestion($catID, $num);
			$list[] = $rec['qID'];
		}
		$now = date('Y-m-d H:i:s');
		return array(
			'cnt'=>$cnt,
			'giveup'=>$giveup,
			'list'=>$list,
			'sysDate'=>$now,
		);
	}
	function success($result) {
		return $result;
	}
}
