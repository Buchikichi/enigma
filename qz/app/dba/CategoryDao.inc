<?php
/**
 * Category情報を管理.
 * @author H.Sasai
 */
class CategoryDao extends DaoBase {
	/**
	 * Categoryを新規登録する.
	 * @param mixed $param
	 * @return number
	 */
	function insert($param) {
		$query = 'INSERT INTO question(catID, content)'
			.' VALUES(:catID, :content)';
		return $this->execute($query, $param);
	}

	/**
	 * Categoryを更新する
	 * @param mixed $param
	 * @return number
	 */
	function update($param) {
		$query = <<<EOD
UPDATE p_beam_item SET
	itemID = :itemID,
	itemName = :itemName,
	orderNum = :orderNum
WHERE
	seq = :seq
EOD;
		return $this->execute($query, $param);
	}

	/**
	 * CatIDを取得する.
	 * @param unknown $catName
	 */
	function getCatID($catName) {
		$query = <<<EOD
SELECT
  *
FROM
  category
WHERE
  catName = :catName
EOD;
		$param = array('catName'=>$catName);
		$rec = $this->selectOne($query, $param);
		return $rec['catID'];
	}

	/**
	 * Categoryの一覧を取得する.
	 */
	function getList($param) {
		$from = intval($param['from']);
		$to = intval($param['to']);
		unset($param['from']);
		unset($param['to']);
		$query = <<<EOD
SELECT
  cat.catID,
  cat.catName,
  count(qID) cnt
FROM
  category cat
LEFT JOIN question que ON
  cat.catID = que.catID
GROUP BY
  cat.catID, cat.catName
ORDER BY
  cat.catName
LIMIT $from, $to
EOD;
		return $this->select($query, $param);
	}
}
?>
