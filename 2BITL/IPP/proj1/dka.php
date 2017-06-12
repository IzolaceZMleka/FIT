<?php

#DKA:xpavli78
/*
 * Soubor: dka.php
 * Datum: 9.3.2016
 * Autor: Jan Pavlica, xpavli78@stud.fit.vutbr.cz
 * Projekt: Determinizace konecneho automatu (DKA)
 * Predmet: IPP
 * Popis: hlavni program, tisk vysledneho automatu
*/

include("args.php");
include("lexer.php");
include("eps_det.php");

mb_internal_encoding("UTF-8");

function print_fsm($states,$rules,$f_states,$i_state,$alphabet,$out_file)
{
	
	
	// sezareni stavu
	
	foreach($rules as $rule)
	{
		if($rule->symbol == "eps")
		{
			$rule->symbol = "";
		}
	}
		
	asort($states);
	asort($rules);
	asort($f_states);
	asort($alphabet);
	
	$first = false;
	
	$out = "(\n{";
	
	foreach($states as $state)
	{
		if($first)
		{
			$out.=", $state";
		}
		else
		{
			$out.="$state";
			$first = true;
		}
	}
	
	$first = false;
	
	$out .= "},\n{";
	
	foreach($alphabet as $a)
	{
		if($a == "'")
			$a = "''";
			
		if($first)
		{
			$out.=", '$a'";
		}
		else
		{
			$out.="'$a'";
			$first = true;
		}
	}
	
	$first = false;
	
	$out .= "},\n{";
	
	foreach($rules as $rule)
	{
		if($rule->symbol == "'")
			$rule->symbol = "''";
			
		if($first)
		{
			$out.=",\n" . $rule->from . " '" . $rule->symbol . "' -> " . $rule->to;
		}
		else
		{
			$out.= "\n" . $rule->from . " '" . $rule->symbol . "' -> " . $rule->to;
			$first = true;
		}
	}
	
	$out .= "\n},\n$i_state,\n{";
	
	$first = false;
	
	foreach($f_states as $f_state)
	{
		if($first)
		{
			$out.=", $f_state";
		}
		else
		{
			$out.="$f_state";
			$first = true;
		}
	}
	
	$out .= "}\n)";
	
	if(!file_put_contents($out_file, $out)){
        exit(3);
    }
	
}

$eps = false;
$det = false;
$sens = false;
$in_file = "";
$out_file = "";
unset($argv[0]);


get_params($argc, $argv, $eps, $det, $sens, $in_file, $out_file);

$file_content = file_get_contents($in_file);

if(!$file_content)
	exit(2);
	
if($sens)
{
	$file_content = mb_strtolower($file_content);
}
	
//echo "$file_content";

$alphabet = array();
$states = array();
$i_state = "";
$rules = array();
$f_states = array();

get_fsm($file_content, $alphabet, $states, $i_state, $rules, $f_states, $sens);

// var_dump($rules);

if($eps)
{
	fu_eps($states,$rules,$f_states);
}
elseif($det)
{
	fu_determinization($states,$rules,$f_states,$i_state,$alphabet);
}


// var_dump($states);
// var_dump($alphabet);
// var_dump($rules);
// echo "\n $i_state \n";
// var_dump($f_states);

print_fsm($states,$rules,$f_states,$i_state,$alphabet,$out_file);

?>
