import React from 'react';

import { DetailsList } from 'office-ui-fabric-react/lib/DetailsList';
import styled from 'styled-components';

import theme from 'theme';

const StyledTable = styled(DetailsList)`

    margin-top: 0rem;

    .ms-DetailsHeader {
        background-color: ${theme.neutralPrimaryAlt};
        color: ${theme.white};
        padding: 0.4rem 0rem;
        height: auto;
        line-height: 40px;
        
        .ms-DetailsHeader-cell, .ms-DetailsHeader-cell:hover {
            font-weight: 600;
            text-transform: uppercase;
            background-color: ${theme.neutralPrimaryAlt};
            color: ${theme.white};
        }
    }

`;

export default StyledTable;
