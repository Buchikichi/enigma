<?php
/**
 * 次の問題.
 * @author H.Sasai
 */
class JudgeAction extends ActionBase {
	function getParamNames() {
		return array('ticket', 'qID', 'seq');
	}
	function makeData() {
		$qID = $_REQUEST['qID'];
		$seq = $_REQUEST['seq'];
		$description = $_REQUEST['description'];
		$answerDao = new AnswerDao();
		$answer = $answerDao->getAnswer($qID);
		$content = $answer['content'];
		$content = str_replace("\n", "", $content);
		if (!empty($seq)) {
			// 選択式回答
			$isOK = $answer['seq'] == $seq;
		} else {
			// 入力式回答
			$isOK = $description == $content;
		}
		$result = $isOK ? '○正解' : '×不正解';
		$now = date('Y-m-d H:i:s');
		return array(
			'judge'=>$isOK,
			'result'=>$result,
			'answer'=>$content,
			'sysDate'=>$now,
		);
	}
	function success($result) {
		return $result;
	}
}
