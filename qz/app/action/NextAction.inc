<?php
/**
 * 次の問題.
 * @author H.Sasai
 */
class NextAction extends ActionBase {
	function getParamNames() {
		return array('ticket', 'catID');
	}
	function makeData() {
		$catID = $_REQUEST['catID'];
		$questionDao = new QuestionDao();
		$answerDao = new AnswerDao();
		if (isset($_REQUEST['qID'])) {
			$question = $questionDao->getByID($_REQUEST['qID']);
		} else {
			$question = $questionDao->getQuestion($catID);
		}
		$qId = $question['qID'];
		$questionText = nl2br($question['content']);
		// 選択肢
		$list = $answerDao->getList($qId);
		shuffle($list);
		foreach ($list as &$rec) {
			$content = $rec['content'];
			$rec['content'] = str_replace("\\n", "<br/>", $content);
		}
		$now = date('Y-m-d H:i:s');
		return array(
			'qID'=>$qId,
			'source'=>$question['source'],
			'question'=>$questionText,
			'answer'=>$list,
			'sysDate'=>$now,
		);
	}
	function success($result) {
		return $result;
	}
}
