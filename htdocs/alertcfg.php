<?php

include("header.php");

printHeader();

include("setup.php");

// -------------------------
// chaeck login

if (!haveLogin())
{
   echo "<br/><div class=\"infoError\"><b><center>Login erforderlich!</center></b></div><br/>\n";
   die("<br/>");
}

// -------------------------
// establish db connection

mysql_connect($mysqlhost, $mysqluser, $mysqlpass);
mysql_select_db($mysqldb) or die("<br/>DB error");
mysql_query("set names 'utf8'");
mysql_query("SET lc_time_names = 'de_DE'");

// ------------------
// variables

$action = "";

// ------------------
// get post

if (isset($_POST["action"]))
   $action = htmlspecialchars($_POST["action"]);

if ($action == "store")
{

 // ------------------
 // store settings

  $ID    = explode("|", $_POST["cnt"]); 

  for ($i = 1; $i <= intval($_POST["id"]); $i++)
  { 

    $adr   = "Adr(" . $ID[$i] . ")";
    $type  = "Type(" . $ID[$i] . ")";
    $min   = "min(" . $ID[$i] . ")";
    $max   = "max(" . $ID[$i] . ")";
    $int   = "Int(" . $ID[$i] . ")";
    $delta = "Delta(" . $ID[$i] . ")";
    $range = "Range(" . $ID[$i] . ")";
    $madr  = "MAdr(" . $ID[$i] . ")";
    $msub  = "MSub(" . $ID[$i] . ")";
    $mbod  = "MBod(" . $ID[$i] . ")";
    $act   = ($_POST["Act($ID[$i])"]) ? "A" : "D"; 
    
    $time = time();
    $data = " address=\"$_POST[$adr]\", type=\"" . mb_strtoupper($_POST[$type]) . "\", min=\"$_POST[$min]\", max=\"$_POST[$max]\", maxrepeat=\"$_POST[$int]\", delta=\"$_POST[$delta]\", rangem=\"$_POST[$range]\", maddress=\"$_POST[$madr]\", msubject=\"$_POST[$msub]\", mbody=\"$_POST[$mbod]\", state=\"$act\" ";

    if ($i == count($ID)-1 && $_POST[$adr] != "") { 
      $update = "insert into sensoralert set inssp=$time, updsp=$time,"; $where = ""; 
      $insert = mysql_query($update . $data . $where) or die("<br/>Error: " . mysql_error());
    } 
    if ($i < count($ID)-1) {
       $update = "update sensoralert set updsp=$time,"; $where = "where id=" . $ID[$i]; 
       $update = mysql_query($update . $data . $where) or die("<br/>Error: " . mysql_error());
    }
  }

}

// ------------------
// delete entry

if (substr($action,0,6) == "delete") 
   $update = mysql_query("delete from sensoralert where id=" . substr($action,6)) or die("<br/>Error: " . mysql_error());

// ------------------
// setup form

$i = 0; $cnt = "0";
echo "      <form action=" . htmlspecialchars($_SERVER["PHP_SELF"]) . " method=post>\n"; 
echo "        <br/>\n";
echo "        <button class=\"button3\" type=submit name=action value=store>Speichern</button>\n";
echo "        <br/></br>\n";

// ------------------------
// setup items ...

seperator("Benachrichtigung bei bestimmten Sensor-Werten", 0, 1);
echo "        <div class=\"input\" id=\"hlp\" style=\"display:none;\" onClick=\"showContent('hlp')\">\n";
echo "          <span class=\"inputComment\">
                Hier formulierst du die Bedingungen (Alarmwerte) für die einzelnen Sensoren, dabei gilt wieder: Sensor-ID und Typ aus der Tabelle <br />
                'Aufzeichnung' entnehmen und hier eintragen.<br /><br />
                <b>Beispiel:</b> Nachricht wenn die Kesselstellgröße unter 50% sinkt, oder sich mehr als 10% in 1min ändert, aber nicht öfter als alle 5min.<br />
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <b>ID:18&nbsp;&nbsp; Typ:VA&nbsp;&nbsp; min:50&nbsp;&nbsp; 
                max:100&nbsp;&nbsp; Intervall:5&nbsp;&nbsp; Änderung:10&nbsp;&nbsp; Zeitraum:1</b><br />
                <b>Zulässige Werte:</b><br /><b>ID:</b> Zahl (auch Hex) | <b>Typ:</b> UD, VA, DI, DO, W1 | <b>min, max, Änderung:</b> Zahl | <b>
                Intervall, Zeitraum:</b> Zahl (Minuten)<br /><br />
                für Betreff und Text können folgende Platzhalter verwendet werden:<br /> 
                %sensorid% %title% %value% %unit% %min% %max% %repeat% %delta% %range% %time% %weburl%<br />
                mit 'aktiv' aktivierst oder deaktivierst du nur die Benachrichtigung, auf die Steuerung hat dies keinen Einfluss
          </span>\n";
echo "        </div><br/>\n";
seperator("Bedingungen&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span class=\"help\" onClick=\"showContent('hlp')\">(Hilfe)</span>", 0, 2);

$result = mysql_query("select * from sensoralert") or die("<br/>Error: " . mysql_error());
     
  while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
  {
    $ID =  $row['id']; $i++; $a = chr($ID+64);
    $cnt = $cnt . "|" . $row['id']; $s = ($row['state'] == "D") ? "; background-color:#ddd\" readOnly=\"true" : "";
    echo "        <div class=\"input\" >\n";
    echo "          <input type=checkbox name=Act(" . $ID . ")" .(($row['state'] == "A") ? " checked" : "") . " onClick=\"readonlyContent('$a',this)\" onLoad=\"disableContent('$a',this)\"></input> aktiv?\n";
    echo "          ID:<input        class=\"inputEdit2\" id=\"a$a\" style=\"width:77px$s\" type=\"text\" name=\"Adr(" . $ID . ")\"   value=\"" . $row['address'] . "\"></input>&nbsp;\n";
    echo "          Typ:<input       class=\"inputEdit2\" id=\"b$a\" style=\"width:28px$s\" type=\"text\" name=\"Type(" . $ID . ")\"  value=\"" . $row['type'] . "\"></input>&nbsp;\n";
    echo "          min:<input       class=\"inputEdit2\" id=\"c$a\" style=\"width:42px$s\" type=\"text\" name=\"min(" . $ID . ")\"   value=\"" . $row['min'] . "\"></input>&nbsp;\n";
    echo "          max:<input       class=\"inputEdit2\" id=\"d$a\" style=\"width:42px$s\" type=\"text\" name=\"max(" . $ID . ")\"   value=\"" . $row['max'] . "\"></input>&nbsp;\n";
    echo "          Intervall:<input class=\"inputEdit2\" id=\"e$a\" style=\"width:42px$s\" type=\"text\" name=\"Int(" . $ID . ")\"   value=\"" . $row['maxrepeat'] . "\"></input>&nbsp;\n";
    echo "          Änderung:<input  class=\"inputEdit2\" id=\"f$a\" style=\"width:33px$s\" type=\"text\" name=\"Delta(" . $ID . ")\" value=\"" . $row['delta'] . "\"></input>&nbsp;\n";
    echo "          Zeitraum:<input  class=\"inputEdit2\" id=\"g$a\" style=\"width:42px$s\" type=\"text\" name=\"Range(" . $ID . ")\" value=\"" . $row['rangem'] . "\"></input>&nbsp;\n";
    echo "          <br /><br />\n";
    echo "          Empfänger:<input class=\"inputEdit2\" id=\"h$a\" style=\"width:260px$s\" type=\"text\" name=\"MAdr(" . $ID . ")\"  value=\"" . $row['maddress'] . "\"></input>&nbsp;\n";
    echo "          Betreff:<input   class=\"inputEdit2\" id=\"i$a\" style=\"width:450px$s\" type=\"text\" name=\"MSub(" . $ID . ")\"  value=\"" . $row['msubject'] . "\"></input>&nbsp;\n";
    echo "          <br /><br />\n";
    echo "          <span style=\"vertical-align:top\">Inhalt:</span>\n";
    echo "          <textarea        class=\"inputEdit2\"  cols=\"400\" rows=\"7\" id=\"j$a\" style=\"width:805px; position:relative; left:42px$s\" name=\"MBod(" . $ID . ")\">" . $row['mbody'] . "</textarea>&nbsp;\n";
    echo "          <button class=\"button4\" style=\"position:absolute;  margin-top:-76px; margin-left:-866px\" type=submit name=action value=delete$ID onclick=\"return confirmSubmit('diesen Eintrag wirklich löschen?')\">Löschen</button>\n";
    echo "        </div><br />\n";
  }
mysql_close();
$ID++;
    $cnt = $cnt . "|" . $ID; 
    echo "        <div class=\"input\">\n";
    echo "          <input type=checkbox name=Act(" . $ID . ")" . (($row['state'] == "A") ? " checked" : "") . "></input> aktiv?\n";
    echo "          ID:<input        class=\"inputEdit2\"  style=\"width:33px\" type=\"text\" name=\"Adr(" . $ID . ")\"   value=\"" . $row['address'] . "\"></input>&nbsp;\n";
    echo "          Typ:<input       class=\"inputEdit2\"  style=\"width:33px\" type=\"text\" name=\"Type(" . $ID . ")\"  value=\"" . $row['type'] . "\"></input>&nbsp;\n";
    echo "          min:<input       class=\"inputEdit2\"  style=\"width:42px\" type=\"text\" name=\"min(" . $ID . ")\"   value=\"" . $row['min'] . "\"></input>&nbsp;\n";
    echo "          max:<input       class=\"inputEdit2\"  style=\"width:42px\" type=\"text\" name=\"max(" . $ID . ")\"   value=\"" . $row['max'] . "\"></input>&nbsp;\n";
    echo "          Intervall:<input class=\"inputEdit2\"  style=\"width:42px\" type=\"text\" name=\"Int(" . $ID . ")\"   value=\"" . $row['maxrepeat'] . "\"></input>&nbsp;\n";
    echo "          Änderung:<input  class=\"inputEdit2\"  style=\"width:33px\" type=\"text\" name=\"Delta(" . $ID . ")\" value=\"" . $row['delta'] . "\"></input>&nbsp;\n";
    echo "          Zeitraum:<input  class=\"inputEdit2\"  style=\"width:42px\" type=\"text\" name=\"Range(" . $ID . ")\" value=\"" . $row['rangem'] . "\"></input>&nbsp;\n";
    echo "          <br /><br />\n";
    echo "          Empfänger:<input class=\"inputEdit2\" style=\"width:260px\" type=\"text\" name=\"MAdr(" . $ID . ")\"  value=\"" . $row['maddress'] . "\"></input>&nbsp;\n";
    echo "          Betreff:<input   class=\"inputEdit2\" style=\"width:450px\" type=\"text\" name=\"MSub(" . $ID . ")\"  value=\"" . $row['msubject'] . "\"></input>&nbsp;\n";
    echo "          <br /><br />\n";
    echo "          <span style=\"vertical-align:top\">Inhalt:</span>\n";
    echo "          <textarea        class=\"inputEdit2\"  cols=\"400\" rows=\"7\" style=\"width:805px; position:relative; left:42px\" name=\"MBod(" . $ID . ")\">" . $row['mbody'] . "</textarea>&nbsp;\n";
    echo "        </div><br />\n";

echo "        <input type=hidden name=id value=" . ($i+1) . ">\n";
echo "        <input type=hidden name=cnt value=" . $cnt . ">\n";
echo "      </form>\n";

include("footer.php");

?>
