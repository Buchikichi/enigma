<?php
require_once('connect.inc');

class DaoBase {
	private static $dbh = null;

	function __construct() {
		if (self::$dbh == null) {
			self::$dbh = new PDO('mysql:host='.HOST.';dbname='.DBNM.';charset=utf8', USER, PASS);
		}
	}
	/**
	 * トランザクションの開始.
	 * ※MySQLの場合、テーブルのTYPEがInnoDBでなければトランザクションを使えない
	 */
	public function beginTransaction() {
		self::$dbh->beginTransaction();
	}
	public function rollBack() {
		self::$dbh->rollBack();
	}
	public function commit() {
		self::$dbh->commit();
	}
	public function execute($query, &$param) {
		$stmt = self::$dbh->prepare($query);
		$stmt->execute($param);
		if ('00000' != $stmt->errorCode()) {
			print_r($stmt->errorInfo());
			print_r($param);
		}
		return $stmt->rowCount();
	}
	public function getLastInsertId($name = NULL) {
		return self::$dbh->lastInsertId($name);
	}
	public function select($query, &$param) {
		$stmt = self::$dbh->prepare($query);
		$stmt->execute($param);
		if ('00000' != $stmt->errorCode()) {
			print_r($stmt->errorInfo());
			print_r($param);
		}
//$stmt->debugDumpParams();
		return $stmt->fetchAll(PDO::FETCH_ASSOC);
	}
	public function selectOne($query, &$param) {
		$stmt = self::$dbh->prepare($query);
//$stmt->debugDumpParams();
		$stmt->execute($param);
		if ('00000' != $stmt->errorCode()) {
			print_r($stmt->errorInfo());
			print_r($param);
		}
		return $stmt->fetch(PDO::FETCH_ASSOC);
	}
	public function selectCount($query, &$param) {
		$rec = $this->selectOne($query, $param);
		return intval($rec['cnt']);
		//↓PHP 5.2.17 ではエラーになる
		//return intval($this->selectOne($query, $param)['cnt']);
	}

	/**
	 * UUIDを取得する.
	 */
	public function getUUID() {
		$param = array();
		$query = 'SELECT uuid() as ID';
		$rec = $this->selectOne($query, $param);
		return $rec['ID'];
	}
}
?>
