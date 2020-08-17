<?php
namespace App\Presenters;

use Nette,
    Nette\Forms\Container,
    Nette\Application\UI\Form;



class ClenPresenter extends BasePresenter {

    private $database;

    public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }

    protected function startup()
    {
        parent::startup();
        if (!$this->getUser()->isLoggedIn()) {
            $this->redirect('Sign:in');
        }
        if (!$this->getUser()->isInRole('admin') and !$this->getUser()->isInRole('spravce')) {
            $this->error('Stránka nebyla nalezena');
        }
    }

    public function renderDefault()
    {
        $this->template->clenove = $this->database->table('clen');
        $this->template->db = $this->database;
    }

    public function renderEdit($Id)
    {
        $post = $this->database->table('clen')->get($Id);
        if (!$post) {
            $this->error('Stránka nebyla nalezena');
        }
    }

    public function actionRemove($ID_clen)
    {
        $row = $this->database->table('clen')->get($ID_clen);
        if($row->funkce =='pilot')
        {
            $this->database->table('ovlada')->where('ID_clen', $ID_clen)->delete();
        }
        if($row->funkce == 'steward')
        {   
            $pole = $this->database->table('umi')->where('ID_clen', $ID_clen)->fetchPairs('ID_jazyk');
            foreach($pole as $item)
            {
                $this->database->table('umi')->where('ID_jazyk', $item->ID_jazyk)->delete();
            }
        }
        $this->database->table('clen')->where('ID_clen', $ID_clen)->delete();
        $this->flashMessage('Člen byl odstraněn');
        $this->redirect('Clen:');
    }

    protected function createComponentAddnewForm()
    {
        $form = new Form;

        $form->addText('jmeno', 'Jméno: *')
            ->setRequired('Zadej jméno');

        $form->addText('rodnecislo', 'Rodné číslo: ');

        $pohlavi = [
            'muž' => 'Muž',
            'žena' => 'Žena',
        ];

        $form->addSelect('pohlavi', 'Pohlaví: *', $pohlavi)
            ->setPrompt('Zvolte pohlaví');

        $form->addRadioList('list', 'Vyberte funkci: *', ['pilot' => 'Pilot', 'steward' => 'Steward'])
        ->addCondition(Form::EQUAL, 'pilot')
        ->toggle('ttyp')
        ->endCondition()
        ->addCondition(Form::EQUAL, 'steward')
        ->toggle('tjazyk')
        ->endCondition();

        $post = $this->database->table('typ_letadla')->fetchPairs('ID_typ','typ');

        $form->addSelect('typ', 'Typ: *', $post)
            ->setOption('id', 'ttyp')
            ->setPrompt('Zvolte typ letadla');

        $jazyk = $this->database->table('jazyk')->fetchPairs('ID_jazyk','ID_jazyk');


        $form->addCheckboxList('jazyk', 'Jazyk: *', $jazyk)
            ->setOption('id', 'tjazyk');

        $form->addSubmit('send', 'Uložit');
        $form->onSuccess[] = [$this, 'addFormSucceeded'];
        $form->setRenderer(new \App\Model\Bs3FormRenderer);
        return $form;
    }

    public function addFormSucceeded($form, $values)
    {
        $row = $this->database->table('clen')->insert(array(
            'jmeno' => $values->jmeno,
            'rodne_cislo' => $values->rodnecislo,
            'pohlavi' => $values->pohlavi,
            'funkce' => $values->list,
        ));

        if($values->list == 'pilot')
        {
            $this->database->table('ovlada')->insert(array(
                'ID_clen' => $row->ID_clen,
                'ID_typ' => $values->typ,
            ));
        }

        if($values->list == 'steward')
        {
            foreach($values->jazyk as $key)
            {
                $this->database->table('umi')->insert(array(
                    'ID_clen' => $row->ID_clen,
                    'ID_jazyk' => $key,
                ));
            }
        }

        $this->flashMessage('V pořádku', 'success');
        $this->redirect('Clen:');
    }

    protected function createComponentEditForm()
    {
        $Id = $this->getParameter('Id');
        $post = $this->database->table('clen')->get($Id);

        if($post->funkce == 'pilot')
        {
            $role = 'pilot';
            $leta = $this->database->table('ovlada')->where('ID_clen',$post->ID_clen)->fetch()->ID_typ;
        }
        else
        {
            $role = 'steward';
            $umi = $this->database->table('umi')->where('ID_clen',$post->ID_clen)->fetchPairs('ID_jazyk','ID_jazyk');
        }

        $form = new Form;

        $form->addText('jmeno', 'Jméno: *')
            ->setRequired('Zadej jméno')
            ->setDefaultValue($post->jmeno);


        $form->addText('rodnecislo', 'Rodné číslo: ')
        ->setDefaultValue($post->rodne_cislo);


        $pohlavi = [
            'muž' => 'Muž',
            'žena' => 'Žena',
        ];

        $form->addSelect('pohlavi', 'Pohlaví: *', $pohlavi)
            ->setPrompt('Zvolte pohlaví')
            ->setDefaultValue($post->pohlavi);


        $form->addRadioList('list', 'Vyberte funkci: *', ['pilot' => 'Pilot', 'steward' => 'Steward'])
        ->setDefaultValue($role)
        ->addCondition(Form::EQUAL, 'pilot')
        ->toggle('ttyp')
        ->endCondition()
        ->addCondition(Form::EQUAL, 'steward')
        ->toggle('tjazyk')
        ->endCondition();
        


        $post2 = $this->database->table('typ_letadla')->fetchPairs('ID_typ','typ');

        $form->addSelect('typ', 'Typ: *', $post2)
            ->setOption('id', 'ttyp')
            ->setPrompt('Zvolte typ letadla');

        if($post->funkce == 'pilot')
        {
            $form['typ']->setDefaultValue($leta);
        }


        $jazyk = $this->database->table('jazyk')->fetchPairs('ID_jazyk','ID_jazyk');


        $form->addCheckboxList('jazyk', 'Jazyk: *', $jazyk)
            ->setOption('id', 'tjazyk');

        if($post->funkce == 'steward')
        {
            $form['jazyk']->setDefaultValue($umi);
        }


        $form->addSubmit('send', 'Uložit');
        $form->onSuccess[] = [$this, 'editFormSucceeded'];
        $form->setRenderer(new \App\Model\Bs3FormRenderer);
        return $form;
    }

    public function editFormSucceeded($form, $values)
    {

        $ID_clen = $this->getParameter('Id');

        if ($ID_clen) 
        {
            $row = $this->database->table('clen')->get($ID_clen);
            if($row->funkce =='pilot')
            {
                $this->database->table('ovlada')->where('ID_clen', $ID_clen)->delete();
            }
            if($row->funkce == 'steward')
            {   
                $pole = $this->database->table('umi')->where('ID_clen', $ID_clen)->fetchPairs('ID_jazyk');
                foreach($pole as $item)
                {
                    $this->database->table('umi')->where('ID_jazyk', $item->ID_jazyk)->delete();
                }
            }
            $this->database->table('clen')->where('ID_clen', $ID_clen)->delete();


            $row = $this->database->table('clen')->insert(array(
            'jmeno' => $values->jmeno,
            'rodne_cislo' => $values->rodnecislo,
            'pohlavi' => $values->pohlavi,
            'funkce' => $values->list,
            ));

            if($values->list == 'pilot')
            {
                $this->database->table('ovlada')->insert(array(
                    'ID_clen' => $row->ID_clen,
                    'ID_typ' => $values->typ,
                ));
            }

            if($values->list == 'steward')
            {
                foreach($values->jazyk as $key)
                {
                    $this->database->table('umi')->insert(array(
                        'ID_clen' => $row->ID_clen,
                        'ID_jazyk' => $key,
                    ));
                }
            }
        }

        $this->flashMessage('V pořádku', 'success');
        $this->redirect('Clen:');
    }
}