<?php
function encode_json($str) {
	return urldecode(json_encode(url_encode($str)));	
}

/**
 * 
 */
function url_encode($str) {
	if(is_array($str)) {
		foreach($str as $key=>$value) {
			if ($key != urlencode($key)){
				$str[urlencode($key)] = url_encode($value);
				unset($str[$key]);
			}else {
				$str[$key] = url_encode($value);
			}
		}
	} else {
		$str = urlencode($str);
	}
	
	return $str;
}



$collection = 'test_bns_real_gem';
$mongo_conn = new Mongo('mongodb://root:root@127.0.0.1:27017/gamedb');
$mongo_db = $mongo_conn->selectDB('gamedb');

$real_data = $mongo_db->selectCollection($collection)->find();

$real_res = array();
$has_push = false;

foreach ($real_data as $doc) {
	$has_push = false;
	if (isset($doc['game_category']) ){
		foreach ($real_res as $key => $value) {
			if ($value['name'] == $doc['name'] && $value['game_category'] == $doc['game_category']){
				$has_push = true;
			}
		}

		if (!$has_push){
			array_push($real_res, array(
					'_id' => $doc['_id'],
					'consumable' => $doc['consumable'],
					'description' => $doc['description'],
					'game_category' => $doc['game_category'],
					'gear_score' => $doc['gear_score'],
					'item_grade' => $doc['item_grade'],
					'location' => $doc['location'],
					'max_value' => $doc['max_value'],
					'money_cost' => $doc['money_cost'],
					'name' => $doc['name'],
					'price' => $doc['price'],
					'icon' => $doc['icon'],
					'title' => $doc['title'],
					'需要等级' => $doc['需要等级'],
					'main' => '',
					'range' => array(),
					'real_id' => array()
				));
		}
	}
}

#var_dump($real_res);
$tmp_real = array();

foreach ($real_res as $key => $value) {
	$tmp_real = array();
	$attr = array('命中'=>array(),'穿刺'=>array(),'暴击'=>array(),'暴击防御'=>array(),'闪避'=>array(),'格挡'=>array(),'最大生命'=>array());
	$main = array();

	$find_query = array("name"=>$value['name'],"game_category"=>$value['game_category']);
	$tmp_real = $mongo_db->selectCollection($collection)->find($find_query);
	
	/*foreach ($attr as $val) {
		$real_res[$key][$val] = array();
	}*/
	
	foreach ($tmp_real as $doc) {
		foreach ($attr as $k=>$val) {
			if ($doc[$k] != "0" && !in_array($doc[$k], $attr[$k])){
				array_push($attr[$k], $doc[$k]);
			}
		}
		array_push($real_res[$key]['real_id'],$doc['_id']);

		#echo encode_json($doc);
		#echo "\n";
	}

	foreach ($attr as $k => $v) {
		if (count($v) > count($main)){
			$main[$k] = $v;
		}
	}

	foreach ($main as $k => $v) {
		$real_res[$key]['main'] = $k.' '.implode('|',$v);
		unset($attr[$k]);
	}

	foreach ($attr as $k => $v) {
		if (!empty($v)){
			array_push($real_res[$key]['range'],$k.' '.implode('|',$v));
		}	
	}
	//var_dump($attr);

	echo encode_json($real_res[$key]);
	echo "\n";
}



$mongo_conn->close();

?>