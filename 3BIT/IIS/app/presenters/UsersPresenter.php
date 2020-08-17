<?php
namespace App\Presenters;

use Nette,
    Nette\Security\Passwords,
    Nette\Application\UI\Form;



class UsersPresenter extends BasePresenter {

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
        if (!$this->getUser()->isInRole('admin')) {
            $this->error('Stránka nebyla nalezena');
        }
    }


    public function renderDefault()
    {
        $this->template->users = $this->database->table('users');
    }

    public function renderEdit($Id)
    {
        $post = $this->database->table('users')->get($Id);
        if (!$post) {
            $this->error('Stránka nebyla nalezena');
        }
    }

    public function actionRemove($Id)
    {   
        $row = $this->database->table('users')->where('id', $Id)->fetch();
        $this->database->table('users')->where('id', $Id)->delete();
        if($row->role == 'technik')
        {
            $this->database->table('technik')->where('ID_technik', $Id)->delete();
        }
        $this->flashMessage('Uživatel byl úspěšně odstraněn.');
        $this->redirect('Users:');
    }

    public function actionAct($Id)
    {
        $row = $this->database->table('users')->where('id', $Id)->fetch();
       
        if($row->activ)
        {
             $row->update(array(
                'activ' =>0
             ));
            $this->flashMessage('Uživatel byl úspěšně deaktivován.');
            $this->redirect('Users:');
        }
        if(!$row->activ)
        {
            $row->update(array(
                'activ' =>1
             ));
            $this->flashMessage('Uživatel byl úspěšně aktivován.');
            $this->redirect('Users:');
        }

    }

    protected function createComponentEditForm()
    {
        $Id = $this->getParameter('Id');
        $post = $this->database->table('users')->get($Id);
        $form = new Form;
        $form->addText('username', 'Jméno: *')
            ->setRequired()
            ->setDefaultValue($post->username);

        $roles = [
            'admin' => 'Admin',
            'technik' => 'Technik',
            'spravce' => 'Správce',
        ];

        $form->addSelect('role', 'Role: *', $roles)
            ->setPrompt('Zvolte roli')
            ->setDefaultValue($post->role)
            ->setRequired('Zadejte roli');

        $form->addPassword('password', 'Nové heslo: *')
            ->setRequired('Zadejte nové heslo');

        $form->addText('rodne_cislo', 'Rodné číslo:')
            ->setDefaultValue($post->rodne_cislo);

        $pohlavi = [
            'muž' => 'Muž',
            'žena' => 'Žena',
        ];

        $form->addSelect('pohlavi', 'Pohlaví:', $pohlavi)
            ->setDefaultValue($post->pohlavi)
            ->setPrompt('Zvolte pohlaví');

        $form->addText('email', 'Email:')
            ->setDefaultValue($post->email)
            ->addRule(Form::EMAIL, 'Špatný email')
            ->setType('email');

        $form->addSubmit('send', 'Uložit');
        $form->onSuccess[] = [$this, 'editFormSucceeded'];
        $form->setRenderer(new \App\Model\Bs3FormRenderer);

        return $form;
    }

    public function editFormSucceeded($form, $values)
    {

    $Id = $this->getParameter('Id');

    if ($Id) {
        $post = $this->database->table('users')->get($Id);
        $post->update(array(
            'username' => $values->username,
            'password' => Passwords::hash($values->password),
            'role' => $values->role,
        ));
    }

    $this->flashMessage('V pořádku', 'success');
    }


    protected function createComponentAddnewForm()
    {
        $form = new Form;
        $form->addText('username', 'Jméno: *')
            ->setRequired('Zadejte jméno');

        $roles = [
            'admin' => 'Admin',
            'technik' => 'Technik',
            'spravce' => 'Správce',
        ];

        $form->addSelect('role', 'Role: *', $roles)
            ->setPrompt('Zvolte roli')
            ->setRequired('Zadejte roli');

        $form->addPassword('password', 'Heslo: *')
            ->setRequired('Zvolte si heslo')
            ->addRule(Form::MIN_LENGTH, 'Heslo musí mít alespoň %d znaky', 3);

        $form->addPassword('passwordVerify', 'Heslo pro kontrolu: *')
            ->setRequired('Zadejte prosím heslo ještě jednou pro kontrolu')
            ->addRule(Form::EQUAL, 'Hesla se neshodují', $form['password']);

        $form->addText('rodne_cislo', 'Rodné číslo:');

        $pohlavi = [
            'muž' => 'Muž',
            'žena' => 'Žena',
        ];

        $form->addSelect('pohlavi', 'Pohlaví:', $pohlavi)
            ->setPrompt('Zvolte pohlaví');

        $form->addText('email', 'Email:')
            ->addRule(Form::EMAIL, 'Špatný email')
            ->setType('email');

        $form->addSubmit('send', 'Uložit');
        $form->onSuccess[] = [$this, 'addFormSucceeded'];
        $form->setRenderer(new \App\Model\Bs3FormRenderer);
        return $form;
    }

    public function addFormSucceeded($form, $values)
    {
       $row = $this->database->table('users')->insert(array(
            'username' => $values->username,
            'password' => Passwords::hash($values->password),
            'role' => $values->role,
            'rodne_cislo' => $values->rodne_cislo,
            'pohlavi' => $values->pohlavi,
            'email' => $values->email,
            'activ' => '1',
        ));
        if($row->role == 'technik')
        {
            $this->database->table('technik')->insert(array(
                'ID_technik'=> $row->id,
                'jmeno' => $values->username,
                'rodne_cislo' => $values->rodne_cislo,
            ));
        }
        $this->flashMessage('V pořádku', 'success');
    }
   
}