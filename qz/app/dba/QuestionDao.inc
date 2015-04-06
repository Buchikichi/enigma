<?php
/**
 * Question情報を管理.
 * @author H.Sasai
 */
class QuestionDao extends DaoBase {
	/**
	 * Questionを新規登録.
	 * @param mixed $param
	 * @return number
	 */
	function insert($param) {
		$query = 'INSERT INTO question(qID, catID, source, content)'
			.' VALUES(:qID, :catID, :source, :content)';
		return $this->execute($query, $param);
	}

	/**
	 * Questionを更新.
	 * @param mixed $param
	 * @return number
	 */
	function update($param) {
		$query = <<<EOD
UPDATE question SET
	catID = :catID,
	content = :content
WHERE
	qID = :qID
EOD;
		return $this->execute($query, $param);
	}

	/**
	 * Questionを削除
	 * @param unknown $catID
	 */
	function delete($catID) {
		$cnt = 0;
		$list = $this->getList($catID);
		foreach ($list as $rec) {
			$qID = $rec['qID'];
			$param = array('qID'=>$qID);
			$query = 'DELETE FROM question where qID=:qID';
			$this->execute($query, $param);
			$query = 'DELETE FROM answer where qID=:qID';
			$this->execute($query, $param);
			$cnt++;
		}
		return $cnt;
	}

	/**
	 * 件数を取得.
	 * @param string $catID カテゴリーID
	 */
	function count($catID) {
		$param = array('catID'=>$catID);
		$query = 'SELECT count(*) cnt FROM question where catID=:catID';
		return $this->selectCount($query, $param);
	}

	/**
	 * 1件取得する.
	 */
	function getByID($qID) {
		$param = array('qID'=>$qID);
		$query = <<<EOD
SELECT
  *
FROM
  question
WHERE
  qID=:qID
EOD;
		return $this->selectOne($query, $param);
	}

	/**
	 * ランダムに1件取得する.
	 */
	function getQuestion($catID, $offset = -1) {
		if ($offset == -1) {
			$cnt = $this->count($catID);
			$offset = rand(0, $cnt - 1);
		}
		$param = array('catID'=>$catID);
		$query = <<<EOD
SELECT
  *
FROM
  question
WHERE
  catID=:catID
LIMIT $offset, 1
EOD;
		return $this->selectOne($query, $param);
	}

	/**
	 * 一覧を取得する.
	 */
	function getList($catID) {
		$param = array('catID'=>$catID);
		$query = <<<EOD
SELECT
  qID
FROM
  question
WHERE
  catID=:catID
EOD;
		return $this->select($query, $param);
	}
}
?>
