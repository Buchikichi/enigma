<?php
/**
 * User情報を管理.
 * @author H.Sasai
 */
class UserDao extends DaoBase {
	/**
	 * Userを取得.
	 */
	function getRecord($user) {
		$param = array('name'=>$user);
		$query = <<<EOD
SELECT
 *
FROM
  `user`
WHERE
  name = :name
EOD;
		return $this->selectOne($query, $param);
	}
}
