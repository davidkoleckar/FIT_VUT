import React from 'react';
import styled from 'styled-components';

import theme from 'theme';


const StyledHeader = styled.div.attrs({
    className: "ms-bgColor-neutralDark ms-fontColor-white"
})`    
    .ms-Persona {
      &:hover {
        .ms-Persona-primaryText {
          color: ${theme.neutralTertiary}; 
         }
      }
    }
    
    .ms-Persona-primaryText {
      color: ${theme.white};
    }
`;

export default function({ children }) {
    return (
        <StyledHeader>
          <div className="d-flex align-items-center no-gutters">
            <div className="col-auto p-3">
              <span className="">Správce Autobazaru</span>
            </div>
            <div className="col" />
            <div className="col-auto">
              {children}
            </div>
          </div>
        </StyledHeader>
    )
}
