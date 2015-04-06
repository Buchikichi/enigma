<?php
/**
 * Answer情報を管理.
 * @author H.Sasai
 */
class AnswerDao extends DaoBase {
	/**
	 * Answerを新規登録.
	 * @param mixed $param
	 * @return number
	 */
	function insert($param) {
		$query = 'INSERT INTO answer(qID, seq, content)'
			.' VALUES(:qID, :seq, :content)';
		return $this->execute($query, $param);
	}

	/**
	 * Answerを更新.
	 * @param mixed $param
	 * @return number
	 */
	function update($param) {
		$query = <<<EOD
UPDATE answer SET
  content = :content
WHERE
  qID = :qID
  AND seq = :seq
EOD;
		return $this->execute($query, $param);
	}

	/**
	 * Answerを取得.
	 */
	function getAnswer($qID) {
		$param = array('qID'=>$qID);
		$query = <<<EOD
SELECT
  md5(qID + seq) seq,
  content
FROM
  answer
WHERE
  qID = :qID
  AND seq = 0
EOD;
		return $this->selectOne($query, $param);
	}

	/**
	 * Answerの一覧を取得.
	 */
	function getList($qID) {
		$param = array('qID'=>$qID);
		$query = <<<EOD
SELECT
  md5(qID + seq) seq,
  content
FROM
  answer
WHERE
  qID = :qID
ORDER BY
  seq
EOD;
		return $this->select($query, $param);
	}
}
?>
