<?php declare(strict_types=1);
// UTF-8 marker äöüÄÖÜß€

// to do: change name 'KundenStatus' throughout this file
require_once './Page.php';

class KundenStatus extends Page
{
    // to do: declare reference variables for members
    // representing substructures/blocks

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
        $sqlQuery = "SELECT ordered_article.status AS `status`, ordered_article.ordered_article_id AS ordered_id,
        article.name AS `name`, article.price AS price, article.picture AS picture
        FROM ordered_article JOIN ordering ON ordering.ordering_id = ordered_article.ordering_id
        JOIN article ON ordered_article.article_id = article.article_id
        WHERE ordered_article.ordered_article_id IN ($id_values)
        ORDER BY ordering.ordering_id";
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
        $data = $this->getViewData(); //NOSONAR ignore unused $data
        // $this->generatePageHeader('to do: change headline'); //to do: set optional parameters
        // to do: output view of this page
        header("Content-Type: application/json; charset=UTF-8");
        $serializedData = json_encode($data);
        echo $serializedData;
        // $this->generatePageFooter();
    }

    protected function processReceivedData():void
    {
        parent::processReceivedData();
        header("Cache-Control: no-store, no-cache, must-revalidate"); // HTTP/1.1
        header("Expires: Sat, 01 Jul 2000 06:00:00 GMT"); // Datum in der Vergangenheit
        header("Cache-Control: post-check=0, pre-check=0", false); // fuer IE
        header("Pragma: no-cache");
        session_cache_limiter('nocache'); // VOR session_start()!
        session_cache_expire(0);
        session_start();
        // to do: call processReceivedData() for all members
    }

    public static function main():void
    {
        try {
            $page = new KundenStatus();
            $page->processReceivedData();
            $page->generateView();
        } catch (Exception $e) {
            //header("Content-type: text/plain; charset=UTF-8");
            header("Content-type: text/html; charset=UTF-8");
            echo $e->getMessage();
        }
    }
}

// This call is starting the creation of the page.
// That is input is processed and output is created.
KundenStatus::main();

