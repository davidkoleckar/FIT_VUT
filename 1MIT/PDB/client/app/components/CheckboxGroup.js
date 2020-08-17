import React from 'react';
import styled from 'styled-components';

import { Label } from 'office-ui-fabric-react/lib/Label';
import {
  assign,
  css,
  getNativeProps,
  inputProperties
} from '@uifabric/utilities';
import { ChoiceGroup } from 'office-ui-fabric-react/lib/components/ChoiceGroup/ChoiceGroup';
import * as styles from 'office-ui-fabric-react/lib/components/ChoiceGroup/ChoiceGroup.scss';

const CheckboxGroupContainer = styled.div`

  .ms-ChoiceFieldGroup {
    display: flex;
  }

  .ms-ChoiceField {
    height: 106px;
    width: auto;
    flex-grow: 1;
    
    &:last-child {
      margin-right: 0;
    }
  }
  
  .ms-ChoiceField-wrapper {
    flex-grow: 1;
  } 

  .ms-ChoiceField-field {
    padding-bottom: 10px;  
    width: 100%;
  }

  .ms-ChoiceField-field:before {
    border-radius: 0;
  }
  
  .ms-ChoiceField-field:after {
    border-radius: 0;
  }
  
  .ms-ChoiceField-field.is-checked:after {
    border: 6px solid #0078d7;
    right: 7px !important;
    top: 7px !important;
    width: 12px;
    height: 12px;
  }
  
  .ms-ChoiceField-labelWrapper {
  
    .ms-Label {
      font-family: 'Open Sans';
      font-size: 0.8rem;
      line-height: 1.2;    
    }
  }
  
`;


export default class CheckboxGroup extends ChoiceGroup {
  render() {
    let { label, options, className, required, selectedKeys } = this.props;
    let { keyChecked, keyFocused } = this.state;

    return (
      <CheckboxGroupContainer>
        <div role='application' className={ className }>
          <div
            className={ css('ms-ChoiceFieldGroup', styles.root) }
            role='group'
            aria-labelledby={ `${this.props.label ? this._id + '-label' : ''} ${this.props['aria-labelledby'] || ''}` }
          >
            { this.props.label && (
              <Label className={ className } required={ required } id={ this._id + '-label' }>{ label }</Label>
            ) }

            { options.map((option) => {
              let {
              onRenderField = this._onRenderField,
              onRenderLabel = this._onRenderLabel
            } = option;

              // Merge internal props into option
              assign(option, {
              checked: selectedKeys.indexOf(option.key) !== -1,
              disabled: option.disabled || this.props.disabled,
              id: `${this._id}-${option.key}`,
              labelId: `${this._labelId}-${option.key}`,
              onRenderLabel
            });

              return (
              <div
              key={ option.key }
              className={ css('ms-ChoiceField', styles.choiceField, {
              ['ms-ChoiceField--image ' + styles.choiceFieldIsImage]: !!option.imageSrc,
              ['ms-ChoiceField--icon ' + styles.choiceFieldIsIcon]: !!option.iconProps,
              ['is-inFocus ' + styles.choiceFieldIsInFocus]: option.key === keyFocused
            })
            }
              >
              <div className={ css('ms-ChoiceField-wrapper', styles.choiceFieldWrapper) }>
              <input
              ref={ this._resolveRef('_inputElement') }
              id={ option.id }
              className={ css('ms-ChoiceField-input', styles.input) }
              type='checkbox'
              name={ this.props.name || this._id }
              disabled={ option.disabled || this.props.disabled }
              checked={ selectedKeys.indexOf(option.key) !== -1 }
              required={ required }
              onChange={ this._onChange.bind(this, option) }
              onFocus={ this._onFocus.bind(this, option) }
              onBlur={ this._onBlur.bind(this, option) }
              aria-labelledby={ option.id }
              { ...getNativeProps(option, inputProperties) }
              />
              { onRenderField(option, this._onRenderField) }
              </div>
              </div>
              );
            }) }
          </div>
        </div>
      </CheckboxGroupContainer>
    );
  }

  _getKeyChecked(props) {
    if (props.selectedKeys !== undefined) {
      return props.selectedKeys;
    }

    const optionsChecked = props.options.filter((option) => {
      return option.checked;
    });


    if (optionsChecked.length === 0) {
      return undefined;
    } else {
      return optionsChecked
    }
  }

}
