<?php

#DKA:xpavli78
/*
 * Soubor: dka.php
 * Datum: 9.3.2016
 * Autor: Jan Pavlica, xpavli78@stud.fit.vutbr.cz
 * Projekt: Determinizace konecneho automatu (DKA)
 * Predmet: IPP
 * Popis: determinizace, odstraneni epsilon prechodu
*/

function fu_eps(&$states, &$rules, &$f_states) // funkce pro odstraneni eps prechodu
{
	$n_f_states = array(); // nove koncove stavy
	$n_rules = array(); // nove pravidla
	
	// vytvoreni epsilon uzaveru podle algoritmu z IFJ (8/36)
	
	foreach($states as $c_state) // vytvarim uzaver pro kazdy stav jednotlive
	{
		$eps_closure = array($c_state);  //promenna pro stavy v uzaveru
		
		do
		{
			$prev_eps_closure = $eps_closure;
			
			foreach($prev_eps_closure as $eps_state) // projizdim kazdy stav z uzaveru
			{
				foreach($rules as $c_rule)
				{
					if($c_rule->symbol == "eps" && $eps_state == $c_rule->from) // pokud narazim na epsilon prechod
					{
						$in = false;
						
						foreach($prev_eps_closure as $c_eps_state) //kontrola zda jiz neni obsazen
						{
							if($c_eps_state == $c_rule->to)
								$in = true;
						}
						if(!$in) // pokud neni pridam jej do uzaveru
						{
							array_push($eps_closure, $c_rule->to);
						}
					}
				}
			}
		}while($prev_eps_closure != $eps_closure);
		
		// odstraneni epsilon prechodu podle algoritmu z IFJ (10/36)
		
		foreach($eps_closure as $c_eps_closure_state) // pro kazdy stav z uzaveru
		{
			foreach($rules as $c_rule) // projdu vsechna pravidla
			{
				if($c_eps_closure_state == $c_rule->from && $c_rule->symbol != "eps") // pokud se shoduje vychozi stav a zaroven se nejedna o epsilon prechod
				{
					//vytvorim nove pravidlo
					$n_rule = new rule();
					$n_rule->from = $c_state;
					$n_rule->symbol = $c_rule->symbol;
					$n_rule->to = $c_rule->to;
					
					add_rule($n_rules,$n_rule); // pridam nove pravidlo
				}
			}
			
			foreach($f_states as $c_f_state) // pridani novych konecnych stavu
			{
				if($c_eps_closure_state == $c_f_state)
				{
						add_state($n_f_states,$c_f_state);
				}
			}
		}
		
	}
	
	$rules = $n_rules; // nahradim stare stavy novymi
	$f_states = $n_f_states; // nahradim stare stavy novymi
}

// funkce pro determinizace podle IFJ (24/36)

function fu_determinization(&$states, &$rules, &$f_states, $i_state, $alphabet)
{
	fu_eps($states, $rules, $f_states); // odstranim epsilon prechody
	
	$states_new = array($i_state); // stavy pro algoritmus obsahujici zezacaktu pouze pocatecni
	$rules_det = array(); // nove pravidla
	$states_det = array(); // nove stavy
	$f_states_det = array(); // nove finalni stavy
	
	$meta = array(); // pole pro ukladani stavu ze kterych jsou slozene nove vznikle stavy
	
	do
	{
		$c_state = array_pop($states_new); // aktualni stav
		
		array_push($states_det,$c_state); // pridam do stavu
		
		foreach($alphabet as $a) // pro kazdy znak z abecedy
		{
			$combined_states = array(); // stavy ze kterych vznikne novy stav
			$combined_state; // promenna pro novy stav
			
			if(array_key_exists($c_state,$meta)) // kontrola klice, zda stav neni nove vznikly mezistav
			{
				foreach($meta[$c_state] as $next_state)
				{
					foreach($rules as $rule)
						{
							if($next_state == $rule->from && $a == $rule->symbol)
							{
								add_state($combined_states,$rule->to);
							}
						}
				}
			}
			else // pokud se jedna o puvodni stav
			{
				foreach($rules as $rule)
					{
						if($c_state == $rule->from && $a == $rule->symbol)
						{
							add_state($combined_states,$rule->to);
						}
					}
			}
			
			if(count($combined_states)) // pokud byl nalezen alespon jeden stav
			{
				$first = true;
				$new_rule = new rule();
				asort($combined_states); // serazeni stavu, pro lexikograficke spojeni
				
				foreach ($combined_states as $c_s) // jednotlive stavy spojim a pridam "_"
				{
					if($first)
					{
						$combined_state = $c_s;
						$first = false;
						continue;
					}
					
					$combined_state .= "_";
					$combined_state .= $c_s;
				}
				
				$new_rule->from = $c_state;
				$new_rule->symbol = $a;
				$new_rule->to = $combined_state;
				
				add_rule($rules_det,$new_rule); // pridani noveho pravidla obsahujici nove vznikly stav
			}
			
			if($combined_state) // pri existenci stavu
			{
				$found = false;
				
				foreach($states_det as $state_det) // kontrola zda-li jiz neexistuje
				{
					if($state_det == $combined_state)
					{
						$found = true;
					}
				}
				
				if(!$found && count($combined_states)) // pokud se jedna o nove vznikly mezistav
				{
					add_state($states_new,$combined_state); // pridam jej
					$meta[$combined_state] = $combined_states; // do pole ulozim stavy ze kterych vznikl a samotny stav pouziji jako klic
				}
				
			}
			
		}
		
		foreach($f_states as $f_state) // pridani finalniho stavu
		{
			if(array_key_exists($c_state,$meta)) // v pripade noveho mezistavu
			{
				foreach($meta[$c_state] as $next_state) // kontrola vsech stavu ze kterych vznikl
				{
					if($next_state == $f_state)
						{
							add_state($f_states_det,$c_state);
						}
				}
			}
			elseif($c_state == $f_state)
			{
				add_state($f_states_det,$c_state);
			}
		}
		
	}while(count($states_new));
	
	// prepsani starych stavu
	
	$states = $states_det;
	$f_states = $f_states_det;
	$rules = $rules_det;
}
?>
