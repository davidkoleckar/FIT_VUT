<?php

/**
 * This file is part of the Kdyby (http://www.kdyby.org)
 *
 * Copyright (c) 2008 Filip Procházka (filip@prochazka.su)
 *
 * For the full copyright and license information, please view the file license.md that was distributed with this source code.
 */

namespace Kdyby\Replicator;

use Nette;
use Nette\Forms\Controls\SubmitButton;
use Nette\Utils\Callback;



/**
 * @author Filip Procházka <filip@prochazka.su>
 * @author Jan Tvrdík
 *
 * @method \Nette\Application\UI\Form getForm()
 * @property \Nette\Forms\Container $parent
 */
class Container extends Nette\Forms\Container
{

	/** @var bool */
	public $forceDefault;

	/** @var int */
	public $createDefault;

	/** @var string */
	public $containerClass = 'Nette\Forms\Container';

	/** @var callable */
	protected $factoryCallback;

	/** @var boolean */
	private $submittedBy = FALSE;

	/** @var array */
	private $created = [];

	/** @var \Nette\Http\IRequest */
	private $httpRequest;

	/** @var array */
	private $httpPost;



	/**
	 * @param callable $factory
	 * @param int $createDefault
	 * @param bool $forceDefault
	 *
	 * @throws \Nette\InvalidArgumentException
	 */
	public function __construct($factory, $createDefault = 0, $forceDefault = FALSE)
	{
		parent::__construct();
		$this->monitor('Nette\Application\UI\Presenter');

		try {
			$this->factoryCallback = Callback::closure($factory);
		} catch (Nette\InvalidArgumentException $e) {
			$type = is_object($factory) ? 'instanceof ' . get_class($factory) : gettype($factory);
			throw new Nette\InvalidArgumentException(
				'Replicator requires callable factory, ' . $type . ' given.', 0, $e
			);
		}

		$this->createDefault = (int)$createDefault;
		$this->forceDefault = $forceDefault;
	}



	/**
	 * @param callable $factory
	 */
	public function setFactory($factory)
	{
		$this->factoryCallback = Callback::closure($factory);
	}



	/**
	 * Magical component factory
	 *
	 * @param \Nette\ComponentModel\IContainer
	 */
	protected function attached($obj)
	{
		parent::attached($obj);

		if (!$obj instanceof Nette\Application\UI\Presenter) {
			return;
		}

		$this->loadHttpData();
		$this->createDefault();
	}



	/**
	 * @param boolean $recursive
	 * @return \ArrayIterator|\Nette\Forms\Container[]
	 */
	public function getContainers($recursive = FALSE)
	{
		return $this->getComponents($recursive, 'Nette\Forms\Container');
	}



	/**
	 * @param boolean $recursive
	 * @return \ArrayIterator|Nette\Forms\Controls\SubmitButton[]
	 */
	public function getButtons($recursive = FALSE)
	{
		return $this->getComponents($recursive, 'Nette\Forms\ISubmitterControl');
	}



	/**
	 * Magical component factory
	 *
	 * @param string $name
	 * @return \Nette\Forms\Container
	 */
	protected function createComponent($name)
	{
		$container = $this->createContainer($name);
		$container->currentGroup = $this->currentGroup;
		$this->addComponent($container, $name, $this->getFirstControlName());

		Callback::invoke($this->factoryCallback, $container);

		return $this->created[$container->name] = $container;
	}



	/**
	 * @return string
	 */
	private function getFirstControlName()
	{
		$controls = iterator_to_array($this->getComponents(FALSE, 'Nette\Forms\IControl'));
		$firstControl = reset($controls);
		return $firstControl ? $firstControl->name : NULL;
	}



	/**
	 * @param string $name
	 *
	 * @return \Nette\Forms\Container
	 */
	protected function createContainer($name)
	{
		$class = $this->containerClass;
		return new $class();
	}



	/**
	 * @return boolean
	 */
	public function isSubmittedBy()
	{
		if ($this->submittedBy) {
			return TRUE;
		}

		foreach ($this->getButtons(TRUE) as $button) {
			if ($button->isSubmittedBy()) {
				return $this->submittedBy = TRUE;
			}
		}

		return FALSE;
	}



	/**
	 * Create new container
	 *
	 * @param string|int $name
	 *
	 * @throws \Nette\InvalidArgumentException
	 * @return \Nette\Forms\Container
	 */
	public function createOne($name = NULL)
	{
		if ($name === NULL) {
			$names = array_keys(iterator_to_array($this->getContainers()));
			$name = $names ? max($names) + 1 : 0;
		}

		// Container is overriden, therefore every request for getComponent($name, FALSE) would return container
		if (isset($this->created[$name])) {
			throw new Nette\InvalidArgumentException("Container with name '$name' already exists.");
		}

		return $this[$name];
	}



	/**
	 * @param array|\Traversable $values
	 * @param bool $erase
	 * @param bool $onlyDisabled
	 * @return \Nette\Forms\Container|Container
	 */
	public function setValues($values, $erase = FALSE, $onlyDisabled = FALSE)
	{
		if (!$this->form->isAnchored() || !$this->form->isSubmitted()) {
			foreach ($values as $name => $value) {
				if ((is_array($value) || $value instanceof \Traversable) && !$this->getComponent($name, FALSE)) {
					$this->createOne($name);
				}
			}
		}

		return parent::setValues($values, $erase, $onlyDisabled);
	}



	/**
	 * Loads data received from POST
	 * @internal
	 */
	protected function loadHttpData()
	{
		if (!$this->getForm()->isSubmitted()) {
			return;
		}

		foreach ((array) $this->getHttpData() as $name => $value) {
			if ((is_array($value) || $value instanceof \Traversable) && !$this->getComponent($name, FALSE)) {
				$this->createOne($name);
			}
		}
	}



	/**
	 * Creates default containers
	 * @internal
	 */
	protected function createDefault()
	{
		if (!$this->createDefault) {
			return;
		}

		if (!$this->getForm()->isSubmitted()) {
			foreach (range(0, $this->createDefault - 1) as $key) {
				$this->createOne($key);
			}

		} elseif ($this->forceDefault) {
			while (iterator_count($this->getContainers()) < $this->createDefault) {
				$this->createOne();
			}
		}
	}



	/**
	 * @param string $name
	 * @return array|null
	 */
	protected function getContainerValues($name)
	{
		$post = $this->getHttpData();
		return isset($post[$name]) ? $post[$name] : NULL;
	}



	/**
	 * @return mixed|NULL
	 */
	private function getHttpData()
	{
		if ($this->httpPost === NULL) {
			$path = explode(self::NAME_SEPARATOR, $this->lookupPath('Nette\Forms\Form'));
			$this->httpPost = Nette\Utils\Arrays::get($this->getForm()->getHttpData(), $path, NULL);
		}

		return $this->httpPost;
	}



	/**
	 * @internal
	 * @param \Nette\Application\Request $request
	 * @return Container
	 */
	public function setRequest(Nette\Application\Request $request)
	{
		$this->httpRequest = $request;
		return $this;
	}



	/**
	 * @return \Nette\Application\Request
	 */
	private function getRequest()
	{
		if ($this->httpRequest !== NULL) {
			return $this->httpRequest;
		}

		return $this->httpRequest = $this->getForm()->getPresenter()->getRequest();
	}



	/**
	 * @param \Nette\Forms\Container $container
	 * @param boolean $cleanUpGroups
	 *
	 * @throws \Nette\InvalidArgumentException
	 * @return void
	 */
	public function remove(Nette\Forms\Container $container, $cleanUpGroups = FALSE)
	{
		if ($container->parent !== $this) {
			throw new Nette\InvalidArgumentException('Given component ' . $container->name . ' is not children of ' . $this->name . '.');
		}

		// to check if form was submitted by this one
		foreach ($container->getComponents(TRUE, 'Nette\Forms\ISubmitterControl') as $button) {
			/** @var \Nette\Forms\Controls\SubmitButton $button */
			if ($button->isSubmittedBy()) {
				$this->submittedBy = TRUE;
				break;
			}
		}

		/** @var \Nette\Forms\Controls\BaseControl[] $components */
		$components = $container->getComponents(TRUE);
		$this->removeComponent($container);

		// reflection is required to hack form groups
		$groupRefl = Nette\Reflection\ClassType::from('Nette\Forms\ControlGroup');
		$controlsProperty = $groupRefl->getProperty('controls');
		$controlsProperty->setAccessible(TRUE);

		// walk groups and clean then from removed components
		$affected = [];
		foreach ($this->getForm()->getGroups() as $group) {
			/** @var \SplObjectStorage $groupControls */
			$groupControls = $controlsProperty->getValue($group);

			foreach ($components as $control) {
				if ($groupControls->contains($control)) {
					$groupControls->detach($control);

					if (!in_array($group, $affected, TRUE)) {
						$affected[] = $group;
					}
				}
			}
		}

		// remove affected & empty groups
		if ($cleanUpGroups && $affected) {
			foreach ($this->getForm()->getComponents(FALSE, 'Nette\Forms\Container') as $container) {
				if ($index = array_search($container->currentGroup, $affected, TRUE)) {
					unset($affected[$index]);
				}
			}

			/** @var \Nette\Forms\ControlGroup[] $affected */
			foreach ($affected as $group) {
				if (!$group->getControls() && in_array($group, $this->getForm()->getGroups(), TRUE)) {
					$this->getForm()->removeGroup($group);
				}
			}
		}
	}



	/**
	 * Counts filled values, filtered by given names
	 *
	 * @param array $components
	 * @param array $subComponents
	 * @return int
	 */
	public function countFilledWithout(array $components = [], array $subComponents = [])
	{
		$httpData = array_diff_key((array)$this->getHttpData(), array_flip($components));

		if (!$httpData) {
			return 0;
		}

		$rows = [];
		$subComponents = array_flip($subComponents);
		foreach ($httpData as $item) {
			$filter = function ($value) use (&$filter) {
				if (is_array($value)) {
					return count(array_filter($value, $filter)) > 0;
				}
				return strlen($value);
			};
			$rows[] = array_filter(array_diff_key($item, $subComponents), $filter) ?: FALSE;
		}

		return count(array_filter($rows));
	}



	/**
	 * @param array $exceptChildren
	 * @return bool
	 */
	public function isAllFilled(array $exceptChildren = [])
	{
		$components = [];
		foreach ($this->getComponents(FALSE, 'Nette\Forms\IControl') as $control) {
			/** @var \Nette\Forms\Controls\BaseControl $control */
			$components[] = $control->getName();
		}

		foreach ($this->getContainers() as $container) {
			foreach ($container->getComponents(TRUE, 'Nette\Forms\ISubmitterControl') as $button) {
				/** @var \Nette\Forms\Controls\SubmitButton $button */
				$exceptChildren[] = $button->getName();
			}
		}

		$filled = $this->countFilledWithout($components, array_unique($exceptChildren));
		return $filled === iterator_count($this->getContainers());
	}



	/**
	 * @param $name
	 * @return \Nette\Forms\Container
	 */
	public function addContainer($name)
	{
		return $this[$name] = new Nette\Forms\Container();
	}



	/**
	 * @param \Nette\ComponentModel\IComponent $component
	 * @param $name
	 * @param null $insertBefore
	 * @return \Nette\ComponentModel\Container|\Nette\Forms\Container
	 */
	public function addComponent(Nette\ComponentModel\IComponent $component, $name, $insertBefore = NULL)
	{
		$group = $this->currentGroup;
		$this->currentGroup = NULL;
		parent::addComponent($component, $name, $insertBefore);
		$this->currentGroup = $group;
		return $this;
	}



	/**
	 * @var bool
	 */
	private static $registered = FALSE;

	/**
	 * @param string $methodName
	 * @return void
	 */
	public static function register($methodName = 'addDynamic')
	{
		if (self::$registered) {
			Nette\Forms\Container::extensionMethod(self::$registered, function () {
				throw new Nette\MemberAccessException;
			});
		}

		Nette\Forms\Container::extensionMethod($methodName, function (Nette\Forms\Container $_this, $name, $factory, $createDefault = 0, $forceDefault = FALSE) {
			$control = new Container($factory, $createDefault, $forceDefault);
			$control->currentGroup = $_this->currentGroup;
			return $_this[$name] = $control;
		});

		if (self::$registered) {
			return;
		}

		SubmitButton::extensionMethod('addRemoveOnClick', function (SubmitButton $_this, $callback = NULL) {
			$_this->setValidationScope(FALSE);
			$_this->onClick[] = function (SubmitButton $button) use ($callback) {
				$replicator = $button->lookup(__NAMESPACE__ . '\Container');
				/** @var Container $replicator */
				if (is_callable($callback)) {
					Callback::invoke($callback, $replicator, $button->parent);
				}
				if ($form = $button->getForm(FALSE)) {
					$form->onSuccess = [];
				}
				$replicator->remove($button->parent);
			};
			return $_this;
		});

		SubmitButton::extensionMethod('addCreateOnClick', function (SubmitButton $_this, $allowEmpty = FALSE, $callback = NULL) {
			$_this->onClick[] = function (SubmitButton $button) use ($allowEmpty, $callback) {
				$replicator = $button->lookup(__NAMESPACE__ . '\Container');
				/** @var Container $replicator */
				if (!is_bool($allowEmpty)) {
					$callback = Callback::closure($allowEmpty);
					$allowEmpty = FALSE;
				}
				if ($allowEmpty === TRUE || $replicator->isAllFilled() === TRUE) {
					$newContainer = $replicator->createOne();
					if (is_callable($callback)) {
						Callback::invoke($callback, $replicator, $newContainer);
					}
				}
				$button->getForm()->onSuccess = [];
			};
			return $_this;
		});

		self::$registered = $methodName;
	}

}
