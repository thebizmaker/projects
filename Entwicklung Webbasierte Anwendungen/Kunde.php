<?php declare(strict_types=1);
// UTF-8 marker äöüÄÖÜß€

// to do: change name 'PageTemplate' throughout this file
require_once './Page.php';

class Kunde extends Page
{
    protected function __construct()
    {
        parent::__construct();
        // to do: instantiate members representing substructures/blocks
    }

    public function __destruct()
    {
        parent::__destruct();
    }

    protected function getViewData():array
    {
        // to do: fetch data for this view from the database
		// to do: return array containing data
        if(empty($_SESSION) || !isset($_SESSION['ordered_article']))
            return array();
        $id_values = '';
        foreach($_SESSION['ordered_article'] as $ordered_id){
            $id_values .= $ordered_id . ', ';
        }
        $id_values = substr($id_values, 0, -2);

        $this->_database->real_escape_string($id_values);
        $sqlQuery = "SELECT ordering.ordering_id AS order_id FROM ordered_article
            NATURAL JOIN ordering WHERE ordered_article.ordered_article_id IN ($id_values) LIMIT 1";
        $recordset = $this->_database->query($sqlQuery);
        if (!$recordset) {
            throw new Exception("Abfrage fehlgeschlagen: " . $this->_database->error);
        }
        $result = array();
        $record = $recordset->fetch_assoc();
        while ($record){
            $result[] = $record;
            $record = $recordset->fetch_assoc();
        }
        $recordset->free();
        return $result;
    }

    protected function generateView():void
    {
        $this->generatePageHeader('Kunde', '../Assets/JS/StatusUpdate.js'); //to do: set optional parameters
        // to do: output view of this page
        $order_id = $this->getViewData()[0]['order_id'] ?? '-';
        echo <<< HTML
            <section class="section">
            <h2>Kunde</h2> <hr>
            <h3> Bestellnummer: #$order_id</h3>
        HTML;
        echo '<div class="wraper">';
        echo <<< HTML
            </div> <hr>
            <div class="div-gpreis-status">
                <p id="totalprice"></p>
            </div>
        </section>

        HTML;
        $this->generatePageFooter();
    }

    protected function processReceivedData():void
    {
        parent::processReceivedData();
        // to do: call processReceivedData() for all members
    }

    public static function main():void
    {
        try {
            session_start();
            $page = new Kunde();
            $page->processReceivedData();
            $page->generateView();
        } catch (Exception $e) {
            //header("Content-type: text/plain; charset=UTF-8");
            header("Content-type: text/html; charset=UTF-8");
            echo $e->getMessage();
        }
    }
}

Kunde::main();

