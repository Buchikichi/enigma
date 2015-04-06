<?php
/**
 * 定型処理を行うための基底クラス.
 * @author ponta
 */
abstract class ActionBase {
	var $cause;

	abstract function getParamNames();

	protected function getParameter($key) {
		return isset($_REQUEST[$key]) ? $_REQUEST[$key] : null;
	}

	private function validate() {
		$isOK = true;
		$nameList = $this->getParamNames();
		foreach ($nameList as $name) {
			if (!isset($_REQUEST[$name])) {
				$isOK = false;
				break;
			}
			if ($name == 'ID' && empty($_REQUEST[$name])) {
				$isOK = false;
				break;
			}
		}
		if ($isOK && in_array('seq', $nameList)) {
			$seq = $_REQUEST['seq'];
			$num = ''.intval($seq);
			if ($num == $seq) {
				$isOK = false;
			} else if (strlen($seq) % 2 != 0) {
				$isOK = false;
			}
		}
		return $isOK;
	}

	abstract function makeData();
	abstract function success($result);
	protected function bad() {
		return array('result' => 'NG');
	}

	public function execute() {
		if (!$this->validate()) {
			$bad = $this->bad();
			$bad['cause'] = 'Bad parameter.';
			echo json_encode($bad);
			Logger::getInstance()->debug($bad);
			return false;
		}
		$result = $this->makeData();
		if ($result == null) {
			$bad = $this->bad();
			$bad['cause'] = $this->cause;
			echo json_encode($bad);
			Logger::getInstance()->debug($bad);
			return false;
		}
		$success = $this->success($result);
		//echo json_encode($success);
		echo json_encode($success, JSON_PRETTY_PRINT);
		// ログへ残す
		if (is_array($success)) {
			if (isset($success['list'])) {
				$cnt = count($success['list']);
				$len = strlen(json_encode($success['list']));
				$success['list'] = array('cnt'=>$cnt, 'len'=>$len); // listは巨大になるので削除
			}
			if (isset($success['receipt'])) {
				$success['receipt'] = 'len='.strlen($success['receipt']); // receiptは巨大になるのでサイズを返す
			}
		}
		Logger::getInstance()->debug($success);
		return true;
	}
}
?>
