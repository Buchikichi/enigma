<?php
define('TXT_DIR', APP_ROOT.'/txt/');
/**
 * 問題更新.
 * @author H.Sasai
 */
class QupAction extends ActionBase {
	var $catDao;
	var $questionDao;

	function getParamNames() {
		return array('ticket', 'catID');
	}

	function updateQuestions($filename, $defaultCatID) {
		$catID = $defaultCatID;
		$answerDao = new AnswerDao();
		$lines = file(TXT_DIR.$filename);
		$params = array();
		$content = '';
		$answer = array();
		$phase = 0;
		$numOfQuestions = 0;
		foreach ($lines as $line_num => $line) {
			$cmd = str_replace(array("\r\n","\n","\r"), '', $line);
			$ch = substr($cmd, 0, 1);
			$word = substr($cmd, 1);
			if ($ch == '@') {
				$catName = $word;
				$catID = $this->catDao->getCatID($catName);
				continue;
			}
			if (strpos($cmd, '--') === 0) {
				if (!empty($content)) {
					$qID = $this->questionDao->getUUID();
					$params = array(
							'qID'=>$qID,
							'catID'=>$catID,
							'source'=>$filename,
							'content'=>$content,
					);
					$numOfQuestions++;
					$this->questionDao->insert($params);
					foreach ($answer as $seq => $answerContent) {
						if (strpos($answerContent, '.') == 1) {
							$answerContent = substr($answerContent, 2);
						}
						$params = array(
								'qID'=>$qID,
								'seq'=>$seq,
								'content'=>$answerContent,
						);
						$answerDao->insert($params);
					}
				}
				$content = '';
				$answer = array();
				$phase = 0;
				continue;
			}
			if ($cmd == '/') {
				$phase = 1;
				continue;
			}
			if ($ch == '&') {
				$path = strtolower($catName);
				$content .= '<img src="img/'.$path.'/'.$word.'"/>';
				continue;
			}
			if ($phase == 0) {
				$content .= $line;
			} else if ($phase == 1) {
				$answer[] = $line;
			}
		}
		return $numOfQuestions;
	}

	function makeData() {
		$catID = $_REQUEST['catID'];
		if (empty($catID)) {
			$this->cause = 'Bad catID.';
			return null;
		}
		$this->catDao = new CategoryDao();
		$this->questionDao = new QuestionDao();
		$this->questionDao->beginTransaction();
		$this->questionDao->delete($catID);
		$fileList = array();
		$files = scandir(TXT_DIR);
		foreach ($files as $filename) {
			if (strpos($filename, '.txt') === false) {
				continue;
			}
			$cnt = $this->updateQuestions($filename, $catID);
			$fileList[] = array('file'=>$filename, 'count'=>$cnt);
		}
		$this->questionDao->commit();
		$now = date('Y-m-d H:i:s');
		return array(
			'files'=>$fileList,
			'sysDate'=>$now,
		);
	}

	function success($result) {
		return $result;
	}
}
?>
